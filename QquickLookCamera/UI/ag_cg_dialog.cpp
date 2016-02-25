#include "ag_cg_dialog.h"

AG_CG_Dialog::AG_CG_Dialog(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("set AGCG:"));
	label = new QLabel(this);
	label->setText(tr("Please choose:"));
	combox = new QComboBox(this);
	//combox->setGeometry(50, 50, 80, 30);
	combox->addItem("0");
	combox->addItem("1");
	combox->addItem("2");
	combox->addItem("3");
	combox->addItem("4");
	combox->addItem("5");
	combox->addItem("6");
	combox->addItem("7");
	combox->addItem("8");
	combox->addItem("9");
	combox->addItem("10");
	connect(combox, SIGNAL(activated(int)), this, SLOT(getNumber()));
	vBoxLayout = new QHBoxLayout;
	vBoxLayout->addWidget(label);
	vBoxLayout->addWidget(combox);
	setLayout(vBoxLayout);
}

AG_CG_Dialog::~AG_CG_Dialog()
{

}
void AG_CG_Dialog::getNumber()
{
	QString str = combox->currentText();
	Command *command = new Command(4567, inet_addr("192.168.1.1"), 3955, inet_addr("192.168.1.2"));
	bool ok1;
	int hex = str.toInt(&ok1, 10); 
	command->setAG_CG(hex);
	//close();
}