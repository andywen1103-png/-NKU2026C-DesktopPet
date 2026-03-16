#ifndef PET_MENU_H
#define PET_MENU_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QSlider;
class QPushButton;
class QLabel;
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

private slots:
    void onRoleChanged(int index);
    void onTimeToggled(bool checked);
    void onWeatherToggled(bool checked);
    void onTodosToggled(bool checked);
    void onAddTodoClicked();
    void onClearAllClicked();
    void onVolumeChanged(int value);

private:
    QComboBox *m_roleCombo;
    QCheckBox *m_timeCheck;
    QCheckBox *m_weatherCheck;
    QCheckBox *m_todosCheck;
    QPushButton *m_addTodoBtn;
    QPushButton *m_clearTodosBtn;
    QSlider *m_volumeSlider;
};

#endif // PET_MENU_H
