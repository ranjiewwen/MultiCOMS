#ifndef IMAGEMAGNIFIERITEM_H
#define IMAGEMAGNIFIERITEM_H

/// \brief ImageMagnifierItem provides a window magnifying the area around the mouse

#include <memory>
#include <QWidget>
#include "IWindowItem.h"
#include <QMouseEvent>
#include "../QData/IDataItem.h"
#include "./Utility/PixelConvertor.h"
#include "../SaveFile/SaveToFile.h"
class ImageMagnifierItem : public QWidget, public IWindowItem
{
    Q_OBJECT

protected:
    QPoint m_cursorPostion{0,0}; //zoom in center
    QSize m_range{100,100}; //
    QImage* m_image{nullptr}; //
    std::shared_ptr<unsigned char> m_imgBuffer{nullptr};
protected:
    virtual void paintEvent(QPaintEvent *) override;
private:
	int cmosNumber;
	QImage::Format strFormat;

protected:
    virtual int displayModeDisplay() const override;

public:
	virtual int saveToFile() const override;
	virtual void setSavePath(QString fpath) override;

public:
	ImageMagnifierItem(QWidget* parent = nullptr, Qt::WindowFlags f = 0, QImage::Format str = QImage::Format_ARGB32);//不带默认形参的只能放在带默认形参的前面
    ~ImageMagnifierItem();
	void setCmosNumber(int number);
    virtual IWindowItem* clone() const override;
    virtual int setDataItemPtr(std::shared_ptr<IDataItem>& ptr)  override;
    virtual int refresh() override;	
    virtual QWidget* widget() /*const*/ override {return this;}

public slots:
    void onCursorPositionChanged(int x, int y)
    {
		m_cursorPostion.rx() = x;
        m_cursorPostion.ry() = y;	
        QWidget::update();		
    }	
    void setMagnifierRange(int width, int height); //set range of zoom in area

};

#endif // IMAGEMAGNIFIERITEM_H
