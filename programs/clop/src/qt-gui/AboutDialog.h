#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog {
	Q_OBJECT
      public:
	AboutDialog(QWidget *parent = 0);
	~AboutDialog();

      protected:
	void changeEvent(QEvent *e);

      private:
	Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
