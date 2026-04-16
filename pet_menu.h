#ifndef PET_MENU_H
#define PET_MENU_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QSlider;
class QToolButton;
class QLabel;
class QPushButton;   // 新增
QT_END_NAMESPACE

class PetMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PetMenu(QWidget *parent = nullptr);

signals:
    void roleSelected(int index);
    void timeDisplayToggled(bool checked);
    void weatherDisplayToggled(bool checked);
    void todosDisplayToggled(bool checked);
    void addTodoClicked();
    void clearAllTodosClicked();
    void volumeChanged(int value);
    void citySelected(const QString &cityName);  // 用户确认城市时发射

private slots:
    void onRoleChanged(int index);
    void onTimeToggled(bool checked);
    void onWeatherToggled(bool checked);
    void onTodosToggled(bool checked);
    void onAddTodoClicked();
    void onClearAllClicked();
    void onVolumeChanged(int value);
    void onGoClicked();                          // 新增：点击 Go 按钮

private:
    QComboBox *m_roleCombo;
    QCheckBox *m_timeCheck;
    QCheckBox *m_weatherCheck;
    QCheckBox *m_todosCheck;
    QToolButton *m_addTodoBtn;
    QToolButton *m_clearTodosBtn;
    QSlider *m_volumeSlider;
    QLabel *m_volumeValueLabel;
    QComboBox *m_cityCombo;      // 用于输入/选择城市
    QPushButton *m_goBtn;        // 新增：查询按钮
};

#endif // PET_MENU_H
