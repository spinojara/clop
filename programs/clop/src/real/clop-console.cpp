/////////////////////////////////////////////////////////////////////////////
//
// clop-console.cpp
//
// Rémi Coulom
//
// May, 2010
//
/////////////////////////////////////////////////////////////////////////////
#include "CExperimentSettings.h"
#include "CExperimentFromSettings.h"
#include "CRealObserver.h"
#include "CMESampleMean.h"
#include "CRealExperiment.h"

#include <iostream>
#include <QCoreApplication>
#include <csignal>

class CRealExperiment *rexp;
void sigint_handler(int num) {
	std::cout << "Soft stopping" << std::endl;
	rexp->SoftStop();
}

/////////////////////////////////////////////////////////////////////////////
// Read a .clop file and start experiment
/////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "usage: " << argv[0] << " [r|c]\n";
		std::cerr << " r: replay data file\n";
		std::cerr << " c: start or continue experiment\n";
		std::cerr << ".clop experiment description is read from standard input.\n";
		std::cerr << "example: " << argv[0] << " c <MyExperiment.clop\n";
		return 1;
	}

	const CExperimentSettings es(std::cin);
	CExperimentFromSettings efs(es);

	switch (*argv[1]) {
	case 'c': /////////////////////////////////////////////////////////////////
	{
		efs.QuickLoad();
		CMESampleMean mesm(efs.reg);
		CRealObserver robs(efs.results, efs.paramcol, mesm, efs.reg);
		
		rexp = &efs.rexp;

		QCoreApplication a(argc, argv);
		a.connect(&efs.GetExperiment(), SIGNAL(Finished()), SLOT(quit()));
		efs.GetExperiment().Run();
		std::cout << "Send SIGUSR1 to soft stop." << std::endl;

		signal(SIGUSR1, &sigint_handler);
		return a.exec();
	}

	case 'r': /////////////////////////////////////////////////////////////////
	{
		CMESampleMean mesm(efs.reg);
		CRealObserver robs(efs.results, efs.paramcol, mesm, efs.reg);
		efs.GetExperiment().StartLogs();
		return 0;
	}
	}
}
