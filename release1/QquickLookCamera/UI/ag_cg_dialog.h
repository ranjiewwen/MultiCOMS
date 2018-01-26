#ifndef AG_CG_DIALOG_H
#define AG_CG_DIALOG_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include "ui_ag_cg_dialog.h"
#include "../instruct/Command.h"
class AG_CG_Dialog : public QWidget
{
	Q_OBJECT

public:
	AG_CG_Dialog(QWidget *parent = 0);
	~AG_CG_Dialog();
private slots :
    void  getNumber();
private:
	Ui::Form/*AG_CG_Dialog*/ ui;

private:
	QLabel *label;
	QComboBox *combox;
	QHBoxLayout *vBoxLayout;
};

#endif // AG_CG_DIALOG_H
