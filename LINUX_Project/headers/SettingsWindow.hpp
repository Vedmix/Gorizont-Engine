class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWindow
        (QWidget *parent = nullptr);
signals:
    void backToMenu();
protected:
    void initSliders();
    void initRadioButtons();
    void initButtons();

    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void onBackButtonClicked();
private:
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QVBoxLayout *slidersLayout = new QVBoxLayout();
    QHBoxLayout *radioButtonsLayout = new QHBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    const std::vector<QStrings> sliderNames = {
        "FOV",
        "Graphics",
        "Distance",
        "Speed"
    };

    const std::vector<QStrings> radioButtonNames = {
        "DRUGS MOD"
    };

    const std::vector<QStrings> buttonNames = {
        "Сохранить",
        "По умолчанию",
        "Выход"
    };
};