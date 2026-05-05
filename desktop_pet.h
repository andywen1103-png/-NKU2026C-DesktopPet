#ifndef DESKTOP_PET_H
#define DESKTOP_PET_H

#include <QWidget>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFrame>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMovie>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
QT_END_NAMESPACE

class PetMenu;
class TodoDialog;

struct TodoItem {
    QString name;
    QDateTime dueTime;
    QLabel *displayLabel;
};

class DesktopPet : public QWidget
{
    Q_OBJECT

public:
    explicit DesktopPet(QWidget *parent = nullptr);
    ~DesktopPet();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onLongPress();
    void updateDateTime();
    void updateWeather();
    void switchRole(int index);
    void toggleTimeDisplay(bool checked);
    void toggleWeatherDisplay(bool checked);
    void toggleTodosDisplay(bool checked);
    void onAddTodo();
    void onClearAllTodos();
    void setVolume(int volume);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void checkTodos();
    void toggleMenu();
    void toggleCompanionMode();
    void onCitySelected(const QString &cityName);
    void onWeatherReplyFinished(QNetworkReply *reply);
    void onGifFrameChanged();   // 新增：处理 GIF 每一帧去白底

private:
    void setupUI();
    void loadRoles();
    void setCurrentRole(int index);
    void setPetImage(const QString &state);
    void showBubbleWithPattern(const QString &pattern);
    void refreshTodoList();
    void updateWindowSize();
    void requestWeatherForCity(const QString &cityName);

    QFrame *m_mainFrame;
    QLabel *m_petLabel;
    QLabel *m_timeLabel;
    QLabel *m_weatherLabel;
    QLabel *m_todoTitleLabel;
    QVBoxLayout *m_todoLayout;
    QWidget *m_todoGroup;
    QLabel *m_bubbleLabel;
    QWidget *m_leftPanel;
    PetMenu *m_menu;

    QPushButton *m_closeBtn;
    QPushButton *m_minimizeBtn;
    QPushButton *m_toggleMenuBtn;
    QPushButton *m_companionBtn;

    QPoint m_dragPosition;
    bool m_mousePressed;
    QTimer *m_longPressTimer;
    static const int LONG_PRESS_INTERVAL = 1000;

    struct RoleData {
        QString normalImage;
        QString clickImage;
        QString doubleClickImage;
        QString longPressImage;
        QString musicPath;
        bool isGif;
    };
    QList<RoleData> m_roles;
    int m_currentRoleIndex;

    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    bool m_musicEnabled;

    QTimer *m_timeTimer;
    QTimer *m_weatherTimer;
    QTimer *m_todoTimer;
    QTimer *m_resetTimer;

    QNetworkAccessManager *m_weatherNetManager;
    QString m_apiKey;
    QString m_currentCity;

    QList<TodoItem> m_todos;
    bool m_todosVisible;

    bool m_companionMode;
    int m_previousRoleIndex;
    QMovie *m_gifMovie;
};

#endif // DESKTOP_PET_H
