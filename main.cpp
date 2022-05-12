#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>

class ButtonClick : public QPushButton
{
public:
    explicit ButtonClick(const QString& text = "", QWidget* parent = Q_NULLPTR)
        : QPushButton(parent)
    {
        setText(text);
        setFixedSize(60, 60);

        QFont font;
        font.setPixelSize(16);
        setFont(font);
    }

    void rounding()
    {
        QRect rect(2,2,56, 56);
        QRegion region(rect, QRegion::Ellipse);
        setMask(region);
    }
};

class DoublePB : public QWidget
{
    Q_OBJECT

public:
    explicit DoublePB(const QString& firstText = "",
                      const QString& secondText = "",
                      QWidget* parent = Q_NULLPTR)
        : QWidget(parent)
    {
        setFixedSize(61, 125);
        setLayout(new QVBoxLayout);

        auto* fButton = new ButtonClick(firstText);
        auto* sButton = new ButtonClick(secondText);

        layout()->addWidget(fButton);
        layout()->setMargin(0);
        layout()->addWidget(sButton);

        connect(fButton, &QPushButton::clicked, this, &DoublePB::increase);
        connect(sButton, &QPushButton::clicked, this, &DoublePB::decrease);
    }

signals:
    void increase();
    void decrease();
};

class Display : public QTextEdit
{
public:
    explicit Display(QWidget* parent = Q_NULLPTR)
        : QTextEdit(parent)
    {
        setReadOnly(true);

        QFont font;
        font.setPixelSize(22);
        setFont(font);
    }

    void getText(int channel, int volume)
    {
        setText("Channel: " + QString::number(channel));
        append("Volume: " + QString::number(volume) + "%");
    }
};

class RemoteControl : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteControl(QWidget* parent = Q_NULLPTR)
        : QWidget(parent), channel(0), volume(50), display(new Display), idTimer(0)
    {
        setFixedSize(240, 680);

        auto* remLay = new QVBoxLayout;
        setLayout(remLay);

        auto* layNumb = new QGridLayout;
        remLay->addLayout(layNumb);

        //================================== Ц И Ф Р Ы ====================================
        for(int i = 0; i < 10; ++i)
        {
            auto* numberClick = new ButtonClick(QString::number(i));
            numberClick->rounding();

            connect(numberClick, &ButtonClick::clicked, [i, this]()
            {
                if(idTimer)
                {
                    killTimer(idTimer);
                    idTimer = 0;
                }
                else channel = 0;

                if(channel < 100)
                    channel = (channel * 10) + i;

                idTimer = startTimer(1000);
            });

            if(i == 0)
                layNumb->addWidget(numberClick, 3, 1);
            else
                layNumb->addWidget(numberClick, (i - 1) / 3, (i - 1) % 3);
        }

        //================================= К А Ч Е Л И ===================================
        auto* hLay = new QHBoxLayout;
        remLay->addLayout(hLay);

        auto* volControl = new DoublePB("vol +", "vol -");
        auto* chanControl = new DoublePB("chan +", "chan -");

        connect(volControl, &DoublePB::increase, [this]()
        {
            if(volume < 100)
                volume += 10;

            display->getText(channel, volume);
        });

        connect(volControl, &DoublePB::decrease, [this]()
        {
            if(volume > 0)
                volume -= 10;

            display->getText(channel, volume);
        });

        connect(chanControl, &DoublePB::increase, [this]()
        {
            if(channel < 999)
                channel++;

            display->getText(channel, volume);
        });

        connect(chanControl, &DoublePB::decrease, [this]()
        {
            if(channel > 0)
                channel--;

            display->getText(channel, volume);
        });

        hLay->addWidget(volControl);
        hLay->addWidget(chanControl);

        //================================ Д И С П Л Е Й ===================================
        display->getText(channel, volume);
        remLay->addSpacing(100);
        remLay->addWidget(display);
    }

protected:
    virtual void timerEvent(QTimerEvent* event) override
    {
        display->getText(channel, volume);
        killTimer(idTimer);
        idTimer = 0;
    }

private:
    int channel;
    int volume;

    Display* display;

    int idTimer;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    RemoteControl remContr;
    remContr.show();

    return QApplication::exec();
}

#include "main.moc"