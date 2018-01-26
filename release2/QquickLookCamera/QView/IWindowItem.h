#ifndef IWINDOWITEM_H
#define IWINDOWITEM_H

#include <memory>
#include <QColor>
#include <QSizePolicy>

class PixelConvertor; //convert pixel
class IDataItem; //item contains image data and others additional data
class QWidget; //exports widget
class QMouseEvent;
class MenuProxy; //
class QAction;
class SaveToFile;

struct WindowLayout
{
    int col;//column
    int row;//row
    int colSpan{1}; //default 1
    int rowSpan{1}; //default 1
    int colStretchFactor{1}; //default 1
	int hSizePolicy{ QSizePolicy::Fixed }; //width size Policy
	int vSizePolicy{ QSizePolicy::Fixed }; //height
    int hSize; //if hSizePolicy == Fixed, Minimum, Maximum, specify the width
    int vSize; //ditto, height

    QString windowType; //create new window by name
    QWidget* window;
};

class IWindowItem
{
public:
	//function returns int should return the enum value:
	enum ReturnCode
	{
		EXEC_SUCCESS = 0,		//success
		ERR_UNSUPPORTED,		//an unsupported operation
		ERR_INVALID_PARAM,		//invalid parameter
		ERR_NOT_SET_DATAPTR,	//need IDataItem
		ERR_NOT_SET_CONVERTOR,	//need PixelConvertor
		ERR_INTERNAL_ERROR,		//internal error ( features of data wrong )
		ERR_NOT_SET_FILE		//need SaveToFile to save the picture
	};

protected:
	std::shared_ptr<IDataItem> m_dataProvider{ nullptr }; //data provider
	mutable std::shared_ptr<PixelConvertor> m_convertor{ nullptr };  //clone is a const function
	mutable std::shared_ptr<SaveToFile> m_file{ nullptr };    //clone is a const function
	QColor m_bgColor{ Qt::black }; //background color
	QColor m_lineColor{ Qt::darkYellow }; //line color
	QColor m_fontColor{ Qt::white }; // font color
	QColor m_gridColor{ Qt::darkCyan }; //if need grid cell
	QString m_savePath;
protected:
	//if IWindowItem is in edit mode
	virtual int displayModeEdit() const { return EXEC_SUCCESS; }
	//else
	virtual int displayModeDisplay() const = 0;
	virtual int saveToFile() const = 0;
	virtual void setSavePath(QString fpath) = 0;


public:
	IWindowItem(IWindowItem* parent = 0);
	virtual ~IWindowItem() = 0;

	//disable all copy&assign operation, only explicit clone is valid
	IWindowItem(const IWindowItem& cpy) = delete;
	IWindowItem& operator = (IWindowItem& asg) = delete;

	void setBackgroundColor(const QColor& color) { m_bgColor = color; }
	void setLineColor(const QColor& color) { m_lineColor = color; }
	void setFontColor(const QColor& color) { m_fontColor = color; }
	void setGridColor(const QColor& color) { m_gridColor = color; }

	//construct an object from existing object
	virtual IWindowItem* clone() const = 0;

	//add a child at specified position , and adjust it's size in the layout
	virtual int addLayoutItem(IWindowItem* /*child*/, const WindowLayout& /*layout*/) { return ERR_UNSUPPORTED; }
	//add a floating window
	virtual int addFloatingItem(IWindowItem * /*child*/) { return ERR_UNSUPPORTED; }

	//remove a child
	virtual int removeItem(IWindowItem* /*child*/) { return ERR_UNSUPPORTED; }

	//get parent ptr
	virtual IWindowItem* parent() const { return nullptr; }
	//get child
	virtual IWindowItem* getChildByName(const QString& /*name*/) const { return nullptr; }
	virtual QList<IWindowItem*> children() const { return QList<IWindowItem*>(); }
	//siblings
	virtual QList<IWindowItem*> siblings(const IWindowItem* /*parent*/) const { return QList<IWindowItem*>(); }


	//update all child and self
	virtual int refresh() = 0;

	//bind data provider
	virtual int setDataItemPtr(std::shared_ptr<IDataItem>& ptr) { m_dataProvider = ptr; return EXEC_SUCCESS; }

	//set convertor ptr
	virtual void setConvertor(std::shared_ptr<PixelConvertor>& convertor) { m_convertor = convertor; }
	//set saveFile ptr
	virtual void  setSave(std::shared_ptr<SaveToFile>& file){ m_file = file; }
	//virtual void  setRgbSave(std::shared_ptr<SaveToFile>& rgbFile){ m_rgbFile = rgbFile; }

	////get&set window rectangle : left, top ,width, height
	//virtual int windowRect(int& left, int& top, int& width, int& height) const{}
	//virtual int setWindowRect(int left, int top, int width, int height) {}

 //   //get&set client  rectangle : left, top ,width, height
	//virtual int clientRect(int& left, int& top, int& width, int& height) const {}
	//virtual int setClientRect(int left, int top, int width, int height) {}

    //edit item : change position,size,data item provider...
    virtual int editItem() {return EXEC_SUCCESS;}
    //once edit completes
    virtual int editDone() {return EXEC_SUCCESS;}

    //exports inner widget
    virtual QWidget* widget() /*const*/ {return nullptr;}

    //mouse
    virtual void rightMouseButtonDown(QMouseEvent*){}
    virtual void leftMouseButtonDown(QMouseEvent*){}
    virtual void leftMouseButtonDbClick(QMouseEvent*){}
    virtual void setMenuProxy(MenuProxy*) {}

    //response qaction
    virtual void handleAction(const QAction*) {}
};

#endif // IWINDOWITEM_H
