#ifndef DESKTOP_PET_H
#define DESKTOP_PET_H

#include <QWidget>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMap>
#include <QFrame>
#include <QDateTime>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QCheckBox;
class QSlider;
class QHBoxLayout;
class QVBoxLayout;
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

private:
    void setupUI();
    void loadRoles();
    void setCurrentRole(int index);
    void setPetImage(const QString &state);
    void showBubbleWithPattern(const QString &pattern);
    void refreshTodoList();

    // 窗口控制按钮
    QPushButton *m_minimizeBtn;
    QPushButton *m_closeBtn;

    // 主容器（用于边框）
    QFrame *m_mainFrame;

    // UI 组件
    QLabel *m_petLabel;
    QLabel *m_timeLabel;
    QLabel *m_weatherLabel;
    QLabel *m_todoTitleLabel;
    QVBoxLayout *m_todoLayout;
    QLabel *m_bubbleLabel;
    QWidget *m_leftPanel;
    PetMenu *m_menu;

    // 鼠标事件
    QPoint m_dragPosition;
    bool m_mousePressed;
    QTimer *m_longPressTimer;
    static const int LONG_PRESS_INTERVAL = 1000;

    // 角色数据
    struct RoleData {
        QString normalImage;
        QString clickImage;
        QString doubleClickImage;
        QString longPressImage;
        QString musicPath;
    };
    QList<RoleData> m_roles;
    int m_currentRoleIndex;

    // 音乐播放
    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    bool m_musicEnabled;

    // 定时器
    QTimer *m_timeTimer;
    QTimer *m_weatherTimer;
    QTimer *m_todoTimer;
    QTimer *m_resetTimer;

    // 天气模拟
    QStringList m_weatherConditions;

    // 待办列表
    QList<TodoItem> m_todos;
    bool m_todosVisible;
};

#endif // DESKTOP_PET_H
