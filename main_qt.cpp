#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QComboBox>
#include <QProgressBar>
#include <QGroupBox>
#include <QInputDialog>
#include <QFont>
#include <QFontDatabase>
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QCoreApplication>
#include <QRadialGradient>
#include <QLinearGradient>

#include "FileManager.h"
#include "Student.h"
#include "Validator.h"

#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

// =================== THEME STYLESHEETS ===================

static const QString LIGHT_THEME = R"(
    QMainWindow { background-color: #eef4f2; }
    QWidget { font-family: 'Segoe UI', Arial; font-size: 13px; color: #2a3a35; }
    QLabel#titleLabel { font-size: 22px; font-weight: bold; color: #0e4d45; }
    QLabel#subtitleLabel { font-size: 13px; color: #5a7a70; }
    QPushButton {
        background-color: #1a7a6a; color: white; border: none;
        border-radius: 6px; padding: 8px 18px; font-weight: bold;
    }
    QPushButton:hover { background-color: #15665a; }
    QPushButton:pressed { background-color: #10524a; }
    QPushButton#dangerBtn { background-color: #c0392b; }
    QPushButton#dangerBtn:hover { background-color: #a93226; }
    QPushButton#successBtn { background-color: #1a8a5a; }
    QPushButton#successBtn:hover { background-color: #157a4e; }
    QPushButton#warnBtn { background-color: #d4890a; }
    QPushButton#warnBtn:hover { background-color: #b8760a; }
    QPushButton#themeBtn { background-color: #6a8a80; padding: 6px 14px; }
    QPushButton#themeBtn:hover { background-color: #5a7a70; }
    QLineEdit {
        padding: 8px 12px; border: 2px solid #b8d4ca; border-radius: 6px;
        background: white; color: #2a3a35;
    }
    QLineEdit:focus { border-color: #1a7a6a; }
    QTableWidget {
        background: white; gridline-color: #d0e4dc; border: 1px solid #b8d4ca;
        border-radius: 6px;
    }
    QTableWidget::item { padding: 6px; }
    QTableWidget::item:selected { background-color: #c8e6d8; color: #2a3a35; }
    QHeaderView::section {
        background-color: #d8ece4; color: #0e4d45; font-weight: bold;
        padding: 8px; border: none; border-bottom: 2px solid #8abcaa;
    }
    QTabWidget::pane { border: 1px solid #b8d4ca; border-radius: 6px; background: white; }
    QTabBar::tab {
        padding: 10px 20px; margin-right: 2px; border-top-left-radius: 6px;
        border-top-right-radius: 6px; background: #d8ece4; color: #5a7a70;
    }
    QTabBar::tab:selected { background: #1a7a6a; color: white; }
    QTabBar::tab:hover:!selected { background: #c0dcd2; }
    QProgressBar {
        border: 2px solid #b8d4ca; border-radius: 6px; text-align: center;
        background: #d8ece4;
    }
    QProgressBar::chunk { background-color: #1a8a5a; border-radius: 4px; }
    QGroupBox {
        font-weight: bold; border: 2px solid #b8d4ca; border-radius: 8px;
        margin-top: 10px; padding-top: 15px;
    }
    QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 8px; }
    QComboBox {
        padding: 8px; border: 2px solid #b8d4ca; border-radius: 6px;
        background: white; color: #2a3a35;
    }
)";

static const QString DARK_THEME = R"(
    QMainWindow { background-color: #0a0e14; }
    QWidget { font-family: 'Segoe UI', Arial; font-size: 13px; color: #c8dce8; }
    QLabel#titleLabel { font-size: 22px; font-weight: bold; color: #a8c8f0; }
    QLabel#subtitleLabel { font-size: 13px; color: #6a8aaa; }
    QPushButton {
        background-color: #2a5080; color: white; border: none;
        border-radius: 6px; padding: 8px 18px; font-weight: bold;
    }
    QPushButton:hover { background-color: #3668a0; }
    QPushButton:pressed { background-color: #1e3e66; }
    QPushButton#dangerBtn { background-color: #a93226; }
    QPushButton#dangerBtn:hover { background-color: #c0392b; }
    QPushButton#successBtn { background-color: #1a7a50; }
    QPushButton#successBtn:hover { background-color: #1e9460; }
    QPushButton#warnBtn { background-color: #b87a0a; }
    QPushButton#warnBtn:hover { background-color: #d4890a; }
    QPushButton#themeBtn { background-color: #1e3050; padding: 6px 14px; }
    QPushButton#themeBtn:hover { background-color: #2a4268; }
    QLineEdit {
        padding: 8px 12px; border: 2px solid #1a2a3e; border-radius: 6px;
        background: #0c1420; color: #c8dce8;
    }
    QLineEdit:focus { border-color: #5a9ad0; }
    QTableWidget {
        background: #0c1420; gridline-color: #1a2a3e; border: 1px solid #1a2a3e;
        border-radius: 6px; color: #c8dce8; alternate-background-color: #0a0e14;
    }
    QTableWidget::item { padding: 6px; }
    QTableWidget::item:selected { background-color: #2a5080; color: white; }
    QHeaderView::section {
        background-color: #0e1a2a; color: #a8c8f0; font-weight: bold;
        padding: 8px; border: none; border-bottom: 2px solid #2a5080;
    }
    QTabWidget::pane { border: 1px solid #1a2a3e; border-radius: 6px; background: #0c1420; }
    QTabBar::tab {
        padding: 10px 20px; margin-right: 2px; border-top-left-radius: 6px;
        border-top-right-radius: 6px; background: #0e1a2a; color: #6a8aaa;
    }
    QTabBar::tab:selected { background: #2a5080; color: white; }
    QTabBar::tab:hover:!selected { background: #121e2e; color: #8aa0c0; }
    QProgressBar {
        border: 2px solid #1a2a3e; border-radius: 6px; text-align: center;
        background: #0c1420; color: #c8dce8;
    }
    QProgressBar::chunk { background-color: #3a78b0; border-radius: 4px; }
    QGroupBox {
        font-weight: bold; border: 2px solid #1a2a3e; border-radius: 8px;
        margin-top: 10px; padding-top: 15px; color: #c8dce8;
    }
    QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 8px; }
    QComboBox {
        padding: 8px; border: 2px solid #1a2a3e; border-radius: 6px;
        background: #0c1420; color: #c8dce8;
    }
    QComboBox QAbstractItemView { background: #0c1420; color: #c8dce8; selection-background-color: #2a5080; }
)";

// =================== FORWARD DECLARATIONS ===================
class LoginPage;
class DashboardPage;

// =================== MAIN WINDOW ===================
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void showDashboard();
    void showLogin();
    void toggleTheme();

private:
    FileManager fileManager;
    QStackedWidget *stack;
    LoginPage *loginPage;
    DashboardPage *dashboardPage;
    bool isDarkTheme;
    void applyTheme();
};

// =================== LOGIN PAGE ===================
class LoginPage : public QWidget
{
    Q_OBJECT
public:
    LoginPage(FileManager *fm, QWidget *parent = nullptr);
    void setDarkMode(bool dark);
signals:
    void loginSuccess();
private slots:
    void onLogin();
    void onCreateAccount();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    FileManager *fileManager;
    QLineEdit *studentIdEdit;
    QLineEdit *pinEdit;
    QLabel *statusLabel;
    QWidget *cardWidget;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *idLabel;
    QLabel *pinFieldLabel;
    bool darkMode;
    QPixmap bgCached;
    int bgCachedWidth;
    int bgCachedHeight;
    bool bgCachedDark;
    void generateBackground(int w, int h, bool dark);
};

// =================== DASHBOARD PAGE ===================
class DashboardPage : public QWidget
{
    Q_OBJECT
public:
    DashboardPage(FileManager *fm, QWidget *parent = nullptr);
    void refreshAll();
    void setDarkTheme(bool dark) { darkTheme = dark; }
signals:
    void logoutRequested();
    void themeToggleRequested();
private slots:
    void onSearch();
    void onAddItem();
    void onRefresh();

private:
    FileManager *fileManager;
    bool darkTheme = false;

    void showStyledMsg(const QString &title, const QString &text);
    bool showStyledConfirm(const QString &title, const QString &text);
    int showStyledChoice(const QString &title, const QString &text,
                         const QStringList &buttonLabels);

    QLabel *welcomeLabel;

    // Browse tab
    QLineEdit *searchEdit;
    QTableWidget *browseTable;

    // My Items tab
    QTableWidget *myItemsTable;

    // Requests tab
    QTableWidget *requestsTable;

    // Connections tab
    QTableWidget *connectionsTable;

    // Profile tab
    QLabel *profileInfoLabel;

    void setupBrowseTab(QWidget *tab);
    void setupMyItemsTab(QWidget *tab);
    void setupRequestsTab(QWidget *tab);
    void setupConnectionsTab(QWidget *tab);
    void setupProfileTab(QWidget *tab);

    void refreshBrowse();
    void refreshMyItems();
    void refreshRequests();
    void refreshConnections();
    void refreshProfile();

    void checkWaitlistNotifications();
};

// =================== MAIN WINDOW IMPL ===================

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), isDarkTheme(false)
{
    setWindowTitle("ShareSphere - Student Item Sharing");
    setMinimumSize(900, 650);
    resize(1050, 720);

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    loginPage = new LoginPage(&fileManager, this);
    dashboardPage = new DashboardPage(&fileManager, this);

    stack->addWidget(loginPage);
    stack->addWidget(dashboardPage);

    connect(loginPage, &LoginPage::loginSuccess, this, &MainWindow::showDashboard);
    connect(dashboardPage, &DashboardPage::logoutRequested, this, &MainWindow::showLogin);
    connect(dashboardPage, &DashboardPage::themeToggleRequested, this, &MainWindow::toggleTheme);

    applyTheme();
    stack->setCurrentWidget(loginPage);
}

void MainWindow::showDashboard()
{
    dashboardPage->refreshAll();
    stack->setCurrentWidget(dashboardPage);
}

void MainWindow::showLogin()
{
    fileManager.logout();
    stack->setCurrentWidget(loginPage);
}

void MainWindow::toggleTheme()
{
    isDarkTheme = !isDarkTheme;
    applyTheme();
}

void MainWindow::applyTheme()
{
    qApp->setStyleSheet(isDarkTheme ? DARK_THEME : LIGHT_THEME);
    loginPage->setDarkMode(isDarkTheme);
    dashboardPage->setDarkTheme(isDarkTheme);
}

// =================== LOGIN PAGE IMPL ===================

LoginPage::LoginPage(FileManager *fm, QWidget *parent)
    : QWidget(parent), fileManager(fm), darkMode(true),
      bgCachedWidth(0), bgCachedHeight(0), bgCachedDark(true)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    cardWidget = new QWidget();
    cardWidget->setObjectName("loginCard");
    cardWidget->setFixedWidth(460);
    cardWidget->setStyleSheet(
        "QWidget#loginCard {"
        "  background-color: rgba(20, 20, 40, 200);"
        "  border-radius: 20px;"
        "  border: 1px solid rgba(255,255,255,0.1);"
        "}");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(40);
    shadow->setOffset(0, 8);
    shadow->setColor(QColor(0, 0, 0, 120));
    cardWidget->setGraphicsEffect(shadow);

    QVBoxLayout *cardLayout = new QVBoxLayout(cardWidget);
    cardLayout->setContentsMargins(40, 35, 40, 35);
    cardLayout->setSpacing(6);

    titleLabel = new QLabel("ShareSphere");
    QFont titleFont;
    QStringList funFonts = {"Ink Free", "Fredoka", "Fredoka One", "Baloo 2",
                            "Bubblegum Sans", "Comic Sans MS", "Segoe Print"};
    bool fontSet = false;
    for (const QString &fname : funFonts)
    {
        if (QFontDatabase::hasFamily(fname))
        {
            titleFont = QFont(fname, 46, QFont::Bold);
            fontSet = true;
            break;
        }
    }
    if (!fontSet)
        titleFont = QFont("Segoe UI", 44, QFont::Black);

    titleFont.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #0e4d45;");

    idLabel = new QLabel("Student ID");
    idLabel->setStyleSheet("color: #ccc; font-size: 13px; font-weight: bold; margin-top: 4px;");
    studentIdEdit = new QLineEdit();
    studentIdEdit->setPlaceholderText("Enter 9-digit Student ID");
    studentIdEdit->setMaxLength(9);
    studentIdEdit->setMinimumHeight(42);
    studentIdEdit->setStyleSheet(
        "QLineEdit { background: rgba(255,255,255,0.08); border: 2px solid rgba(255,255,255,0.15);"
        "  border-radius: 10px; padding: 10px 16px; color: #e0e0e0; font-size: 14px; }"
        "QLineEdit:focus { border-color: #1a8a70; background: rgba(255,255,255,0.12); }");

    pinFieldLabel = new QLabel("PIN");
    pinFieldLabel->setStyleSheet("color: #ccc; font-size: 13px; font-weight: bold; margin-top: 4px;");
    pinEdit = new QLineEdit();
    pinEdit->setPlaceholderText("Enter 4-digit PIN");
    pinEdit->setEchoMode(QLineEdit::Password);
    pinEdit->setMaxLength(4);
    pinEdit->setMinimumHeight(42);
    pinEdit->setStyleSheet(
        "QLineEdit { background: rgba(255,255,255,0.08); border: 2px solid rgba(255,255,255,0.15);"
        "  border-radius: 10px; padding: 10px 16px; color: #e0e0e0; font-size: 14px; }"
        "QLineEdit:focus { border-color: #1a8a70; background: rgba(255,255,255,0.12); }");

    QPushButton *loginBtn = new QPushButton("  Login  ");
    loginBtn->setMinimumHeight(46);
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "  stop:0 #0d6b5e, stop:1 #1a8a70);"
        "  color: white; border: none; border-radius: 12px;"
        "  font-size: 16px; font-weight: bold; letter-spacing: 1px; }"
        "QPushButton:hover { background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "  stop:0 #1a8a70, stop:1 #28b89a); }"
        "QPushButton:pressed { background: #0d6b5e; }");

    QPushButton *createBtn = new QPushButton("  Create Account  ");
    createBtn->setMinimumHeight(46);
    createBtn->setCursor(Qt::PointingHandCursor);
    createBtn->setStyleSheet(
        "QPushButton { background: transparent;"
        "  color: #1a8a70; border: 2px solid #1a8a70; border-radius: 12px;"
        "  font-size: 14px; font-weight: bold; letter-spacing: 1px; }"
        "QPushButton:hover { background: rgba(26,138,112,0.15); color: #28b89a; border-color: #28b89a; }"
        "QPushButton:pressed { background: rgba(26,138,112,0.25); }");

    statusLabel = new QLabel("");
    statusLabel->setStyleSheet("color: #ff6b6b; font-weight: bold; font-size: 13px;");
    statusLabel->setAlignment(Qt::AlignCenter);

    cardLayout->addWidget(titleLabel);
    cardLayout->addSpacing(22);
    cardLayout->addWidget(idLabel);
    cardLayout->addWidget(studentIdEdit);
    cardLayout->addSpacing(6);
    cardLayout->addWidget(pinFieldLabel);
    cardLayout->addWidget(pinEdit);
    cardLayout->addSpacing(22);
    cardLayout->addWidget(loginBtn);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(createBtn);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(statusLabel);

    mainLayout->addStretch();
    mainLayout->addWidget(cardWidget, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    connect(loginBtn, &QPushButton::clicked, this, &LoginPage::onLogin);
    connect(createBtn, &QPushButton::clicked, this, &LoginPage::onCreateAccount);
    connect(pinEdit, &QLineEdit::returnPressed, this, &LoginPage::onLogin);
    connect(studentIdEdit, &QLineEdit::returnPressed, [this]()
            { pinEdit->setFocus(); });
}

void LoginPage::setDarkMode(bool dark)
{
    darkMode = dark;
    bgCachedWidth = 0;
    if (dark)
    {
        cardWidget->setStyleSheet(
            "QWidget#loginCard {"
            "  background-color: rgba(10, 14, 22, 210);"
            "  border-radius: 20px;"
            "  border: 1px solid rgba(160,200,240,0.12);"
            "}");
        titleLabel->setStyleSheet("color: #a8c8f0;");
        idLabel->setStyleSheet("color: #8aa8c8; font-size: 13px; font-weight: bold; margin-top: 4px;");
        pinFieldLabel->setStyleSheet("color: #8aa8c8; font-size: 13px; font-weight: bold; margin-top: 4px;");
        studentIdEdit->setStyleSheet(
            "QLineEdit { background: rgba(255,255,255,0.06); border: 2px solid #1a2a40;"
            "  border-radius: 10px; padding: 10px 16px; color: #c8dce8; font-size: 14px; }"
            "QLineEdit:focus { border-color: #5a9ad0; background: rgba(255,255,255,0.10); }");
        pinEdit->setStyleSheet(
            "QLineEdit { background: rgba(255,255,255,0.06); border: 2px solid #1a2a40;"
            "  border-radius: 10px; padding: 10px 16px; color: #c8dce8; font-size: 14px; }"
            "QLineEdit:focus { border-color: #5a9ad0; background: rgba(255,255,255,0.10); }");
    }
    else
    {
        cardWidget->setStyleSheet(
            "QWidget#loginCard {"
            "  background-color: rgba(255, 255, 255, 230);"
            "  border-radius: 20px;"
            "  border: 1px solid rgba(0,0,0,0.12);"
            "}");
        titleLabel->setStyleSheet("color: #0e4d45;");
        idLabel->setStyleSheet("color: #1a3a30; font-size: 13px; font-weight: bold; margin-top: 4px;");
        pinFieldLabel->setStyleSheet("color: #1a3a30; font-size: 13px; font-weight: bold; margin-top: 4px;");
        studentIdEdit->setStyleSheet(
            "QLineEdit { background: #f8faf9; border: 2px solid #8abcaa;"
            "  border-radius: 10px; padding: 10px 16px; color: #1a2a25; font-size: 14px; }"
            "QLineEdit:focus { border-color: #1a7a6a; background: white; }");
        pinEdit->setStyleSheet(
            "QLineEdit { background: #f8faf9; border: 2px solid #8abcaa;"
            "  border-radius: 10px; padding: 10px 16px; color: #1a2a25; font-size: 14px; }"
            "QLineEdit:focus { border-color: #1a7a6a; background: white; }");
    }
    update();
}

void LoginPage::generateBackground(int w, int h, bool dark)
{
    bgCached = QPixmap(w, h);
    QPainter p(&bgCached);
    p.setRenderHint(QPainter::Antialiasing);

    if (dark)
    {
        QLinearGradient base(0, 0, w, h);
        base.setColorAt(0, QColor(8, 12, 20));
        base.setColorAt(0.5, QColor(10, 16, 26));
        base.setColorAt(1, QColor(12, 20, 30));
        p.fillRect(0, 0, w, h, base);
    }
    else
    {
        p.fillRect(0, 0, w, h, QColor(245, 242, 238));
    }

    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    QPen gridPen(dark ? QColor(100, 160, 200, 18) : QColor(140, 28, 28, 55));
    gridPen.setWidth(1);
    p.setPen(gridPen);
    int gridSize = 28;
    for (int x = 0; x < w; x += gridSize)
        p.drawLine(x, 0, x, h);
    for (int y = 0; y < h; y += gridSize)
        p.drawLine(0, y, w, y);

    p.end();
    bgCachedWidth = w;
    bgCachedHeight = h;
    bgCachedDark = dark;
}

void LoginPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    if (bgCachedWidth != width() || bgCachedHeight != height() || bgCachedDark != darkMode)
        generateBackground(width(), height(), darkMode);

    painter.drawPixmap(0, 0, bgCached);
}

void LoginPage::onLogin()
{
    QString id = studentIdEdit->text().trimmed();
    QString pin = pinEdit->text().trimmed();

    if (id.isEmpty() || pin.isEmpty())
    {
        statusLabel->setText("Please enter both Student ID and PIN.");
        return;
    }

    if (fileManager->login(id.toStdString(), pin.toStdString()))
    {
        statusLabel->setText("");
        studentIdEdit->clear();
        pinEdit->clear();
        emit loginSuccess();
    }
    else
    {
        statusLabel->setText("Invalid Student ID or PIN.");
    }
}

void LoginPage::onCreateAccount()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Create Student Account");
    dialog.setMinimumWidth(420);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setPlaceholderText("9-digit Student ID (2100XXXXX)");
    idEdit->setMaxLength(9);

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Full Name");

    QComboBox *deptCombo = new QComboBox();
    for (const auto &dept : Validator::getDepartmentList())
        deptCombo->addItem(QString::fromStdString(dept));

    QLineEdit *phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("Phone (11 digits, starts with 01)");
    phoneEdit->setMaxLength(11);

    QLineEdit *emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Email (optional, .edu for verification)");

    QLabel *resultLabel = new QLabel("");
    resultLabel->setWordWrap(true);

    QPushButton *submitBtn = new QPushButton("Create Account");
    submitBtn->setObjectName("successBtn");

    layout->addWidget(new QLabel("Student ID:"));
    layout->addWidget(idEdit);
    layout->addWidget(new QLabel("Full Name:"));
    layout->addWidget(nameEdit);
    layout->addWidget(new QLabel("Department:"));
    layout->addWidget(deptCombo);
    layout->addWidget(new QLabel("Phone:"));
    layout->addWidget(phoneEdit);
    layout->addWidget(new QLabel("Email (optional):"));
    layout->addWidget(emailEdit);
    layout->addSpacing(10);
    layout->addWidget(submitBtn);
    layout->addWidget(resultLabel);

    FileManager *fm = fileManager;
    connect(submitBtn, &QPushButton::clicked, [&, fm]()
            {
        string sid = idEdit->text().trimmed().toStdString();
        string name = nameEdit->text().trimmed().toStdString();
        string dept = deptCombo->currentText().toStdString();
        string phone = phoneEdit->text().trimmed().toStdString();
        string email = emailEdit->text().trimmed().toStdString();

        if (sid.empty() || name.empty() || phone.empty()) {
            resultLabel->setStyleSheet("color: #e74c3c;");
            resultLabel->setText("Student ID, Name, and Phone are required.");
            return;
        }
        if (!Validator::isValidStudentID(sid)) {
            resultLabel->setStyleSheet("color: #e74c3c;");
            resultLabel->setText("Invalid Student ID format.");
            return;
        }
        if (fm->studentIdExists(sid)) {
            resultLabel->setStyleSheet("color: #e74c3c;");
            resultLabel->setText("Student ID already registered.");
            return;
        }
        if (!Validator::isValidPhone(phone)) {
            resultLabel->setStyleSheet("color: #e74c3c;");
            resultLabel->setText("Invalid phone format (11 digits, starts with 01).");
            return;
        }
        if (!email.empty() && !Validator::isValidEmail(email)) {
            resultLabel->setStyleSheet("color: #e74c3c;");
            resultLabel->setText("Invalid email format.");
            return;
        }

        bool verified = !email.empty() && Validator::isValidEmail(email);

        string yearStr = sid.substr(0, 2);
        int entryYear = stoi(yearStr);
        int studentYear = 26 - entryYear + 1;
        if (studentYear < 1) studentYear = 1;
        if (studentYear > 4) studentYear = 4;

        srand(static_cast<unsigned>(time(0)));
        string pin = to_string(1000 + rand() % 9000);

        Student* newStudent = new Student(sid, pin, name, dept, phone, email, 0, studentYear, verified);

        try {
            if (fm->addUser(newStudent)) {
                resultLabel->setStyleSheet("color: #27ae60; font-size: 14px;");
                resultLabel->setText(
                    QString("Account created!\nYour PIN: %1\nPlease remember this PIN!")
                    .arg(QString::fromStdString(pin)));
                submitBtn->setEnabled(false);
            }
        } catch (const exception& e) {
            resultLabel->setStyleSheet("color: #e74c3c;");
            resultLabel->setText(QString("Error: %1").arg(e.what()));
            delete newStudent;
        } });

    dialog.exec();
}

// =================== DASHBOARD PAGE IMPL ===================

DashboardPage::DashboardPage(FileManager *fm, QWidget *parent)
    : QWidget(parent), fileManager(fm)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 10, 15, 10);

    // Top bar
    QHBoxLayout *topBar = new QHBoxLayout();
    QLabel *appTitle = new QLabel("ShareSphere");
    appTitle->setObjectName("titleLabel");
    welcomeLabel = new QLabel("Welcome!");
    welcomeLabel->setObjectName("subtitleLabel");

    QPushButton *refreshBtn = new QPushButton("Refresh");
    QPushButton *themeBtn = new QPushButton("Light / Dark");
    themeBtn->setObjectName("themeBtn");
    QPushButton *logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("dangerBtn");

    topBar->addWidget(appTitle);
    topBar->addSpacing(12);
    topBar->addWidget(welcomeLabel);
    topBar->addStretch();
    topBar->addWidget(refreshBtn);
    topBar->addWidget(themeBtn);
    topBar->addWidget(logoutBtn);

    mainLayout->addLayout(topBar);
    mainLayout->addSpacing(10);

    // Tabs
    QTabWidget *tabs = new QTabWidget();

    QWidget *browseTab = new QWidget();
    QWidget *myItemsTab = new QWidget();
    QWidget *requestsTab = new QWidget();
    QWidget *connectionsTab = new QWidget();
    QWidget *profileTab = new QWidget();

    setupBrowseTab(browseTab);
    setupMyItemsTab(myItemsTab);
    setupRequestsTab(requestsTab);
    setupConnectionsTab(connectionsTab);
    setupProfileTab(profileTab);

    tabs->addTab(browseTab, "Browse Items");
    tabs->addTab(myItemsTab, "My Items");
    tabs->addTab(requestsTab, "Received Requests");
    tabs->addTab(connectionsTab, "Active Connections");
    tabs->addTab(profileTab, "My Profile");

    mainLayout->addWidget(tabs);

    connect(logoutBtn, &QPushButton::clicked, this, &DashboardPage::logoutRequested);
    connect(themeBtn, &QPushButton::clicked, this, &DashboardPage::themeToggleRequested);
    connect(refreshBtn, &QPushButton::clicked, this, &DashboardPage::onRefresh);
}

// =================== TAB SETUP ===================

void DashboardPage::setupBrowseTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QHBoxLayout *searchBar = new QHBoxLayout();
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search by name, category, or description...");
    QPushButton *searchBtn = new QPushButton("Search");
    searchBar->addWidget(searchEdit);
    searchBar->addWidget(searchBtn);

    browseTable = new QTableWidget();
    browseTable->setColumnCount(7);
    browseTable->setHorizontalHeaderLabels(
        {"Name", "Category", "Description", "Owner", "Status", "Waitlist", "Action"});
    browseTable->horizontalHeader()->setStretchLastSection(false);
    browseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    browseTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    browseTable->setColumnWidth(0, 130);
    browseTable->setColumnWidth(1, 100);
    browseTable->setColumnWidth(3, 120);
    browseTable->setColumnWidth(4, 80);
    browseTable->setColumnWidth(5, 80);
    browseTable->setColumnWidth(6, 130);
    browseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    browseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    browseTable->setAlternatingRowColors(true);
    browseTable->verticalHeader()->setDefaultSectionSize(40);

    layout->addLayout(searchBar);
    layout->addWidget(browseTable);

    connect(searchBtn, &QPushButton::clicked, this, &DashboardPage::onSearch);
    connect(searchEdit, &QLineEdit::returnPressed, this, &DashboardPage::onSearch);
}

void DashboardPage::setupMyItemsTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QPushButton *addBtn = new QPushButton("+ Add New Item");
    addBtn->setObjectName("successBtn");
    addBtn->setMaximumWidth(200);

    myItemsTable = new QTableWidget();
    myItemsTable->setColumnCount(5);
    myItemsTable->setHorizontalHeaderLabels({"Name", "Category", "Description", "Status", "Listed"});
    myItemsTable->horizontalHeader()->setStretchLastSection(true);
    myItemsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    myItemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    myItemsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    myItemsTable->setAlternatingRowColors(true);

    layout->addWidget(addBtn);
    layout->addWidget(myItemsTable);

    connect(addBtn, &QPushButton::clicked, this, &DashboardPage::onAddItem);
}

void DashboardPage::setupRequestsTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QLabel *info = new QLabel("Requests from other students who want to borrow your items:");
    info->setObjectName("subtitleLabel");

    requestsTable = new QTableWidget();
    // ── 6 columns: Request# | From | Item | Duration | Notes | Action ──
    // (Trust Score column removed)
    requestsTable->setColumnCount(6);
    requestsTable->setHorizontalHeaderLabels(
        {"Request#", "From", "Item", "Duration", "Notes", "Action"});
    requestsTable->horizontalHeader()->setStretchLastSection(false);
    requestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    requestsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    requestsTable->setColumnWidth(0, 75);
    requestsTable->setColumnWidth(1, 130);
    requestsTable->setColumnWidth(2, 130);
    requestsTable->setColumnWidth(3, 90);
    requestsTable->setColumnWidth(5, 240);
    requestsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    requestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    requestsTable->setAlternatingRowColors(true);
    requestsTable->verticalHeader()->setDefaultSectionSize(48);

    layout->addWidget(info);
    layout->addWidget(requestsTable);
}

void DashboardPage::setupConnectionsTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QLabel *info = new QLabel("Active borrowing/lending connections:");
    info->setObjectName("subtitleLabel");

    connectionsTable = new QTableWidget();
    connectionsTable->setColumnCount(7);
    connectionsTable->setHorizontalHeaderLabels(
        {"Conn#", "Item", "Role", "Other Party", "Contact", "Email", "Action"});
    connectionsTable->horizontalHeader()->setStretchLastSection(true);
    connectionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    connectionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    connectionsTable->setColumnWidth(0, 60);
    connectionsTable->setColumnWidth(2, 70);
    connectionsTable->setColumnWidth(3, 130);
    connectionsTable->setColumnWidth(4, 110);
    connectionsTable->setColumnWidth(5, 160);
    connectionsTable->setColumnWidth(6, 140);
    connectionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    connectionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connectionsTable->setAlternatingRowColors(true);
    connectionsTable->verticalHeader()->setDefaultSectionSize(40);

    layout->addWidget(info);
    layout->addWidget(connectionsTable);
}

void DashboardPage::setupProfileTab(QWidget *tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setAlignment(Qt::AlignTop);

    QGroupBox *infoGroup = new QGroupBox("Personal Information");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);
    profileInfoLabel = new QLabel("");
    profileInfoLabel->setWordWrap(true);
    profileInfoLabel->setStyleSheet("font-size: 14px; line-height: 1.6;");
    infoLayout->addWidget(profileInfoLabel);

    layout->addWidget(infoGroup);
    layout->addStretch();
}

// =================== REFRESH ===================

void DashboardPage::refreshAll()
{
    User *user = fileManager->getCurrentUser();
    if (!user)
        return;

    welcomeLabel->setText(
        QString("Welcome, %1!").arg(QString::fromStdString(user->getFullName())));

    refreshBrowse();
    refreshMyItems();
    refreshRequests();
    refreshConnections();
    refreshProfile();
    checkWaitlistNotifications();
}

void DashboardPage::onRefresh()
{
    fileManager->loadAllDataFromFiles();
    refreshAll();
}

void DashboardPage::onSearch()
{
    refreshBrowse();
}

// =================== STYLED DIALOG HELPERS ===================

void DashboardPage::showStyledMsg(const QString &title, const QString &text)
{
    QDialog dlg(this);
    dlg.setWindowTitle(title);
    dlg.setMinimumWidth(380);
    dlg.setMaximumWidth(500);

    QString bg, fg, btnBg, btnHover, borderCol;
    if (darkTheme)
    {
        bg = "#f5f2ee";
        fg = "#1a5070";
        btnBg = "#2a5080";
        btnHover = "#3668a0";
        borderCol = "#d0ccc6";
    }
    else
    {
        bg = "#0a0e14";
        fg = "#a8c8f0";
        btnBg = "#2a5080";
        btnHover = "#3668a0";
        borderCol = "#1a2a44";
    }

    dlg.setStyleSheet(QString(
                          "QDialog { background: %1; border: 1px solid %5; border-radius: 12px; }"
                          "QLabel { color: %2; font-size: 14px; background: transparent; }"
                          "QLabel#dlgTitle { font-size: 18px; font-weight: bold; }"
                          "QPushButton { background: %3; color: white; border: none; border-radius: 8px;"
                          "  padding: 10px 28px; font-size: 14px; font-weight: bold; }"
                          "QPushButton:hover { background: %4; }")
                          .arg(bg, fg, btnBg, btnHover, borderCol));

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(30, 25, 30, 25);
    layout->setSpacing(16);

    QLabel *titleLbl = new QLabel(title);
    titleLbl->setObjectName("dlgTitle");
    titleLbl->setAlignment(Qt::AlignCenter);

    QLabel *textLbl = new QLabel(text);
    textLbl->setWordWrap(true);
    textLbl->setAlignment(Qt::AlignCenter);

    QPushButton *okBtn = new QPushButton("OK");
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setFixedHeight(40);
    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);

    layout->addWidget(titleLbl);
    layout->addWidget(textLbl);
    layout->addSpacing(4);
    layout->addWidget(okBtn);

    dlg.exec();
}

bool DashboardPage::showStyledConfirm(const QString &title, const QString &text)
{
    QDialog dlg(this);
    dlg.setWindowTitle(title);
    dlg.setMinimumWidth(380);
    dlg.setMaximumWidth(500);

    QString bg, fg, yesBg, yesHover, noBg, noHover, borderCol;
    if (darkTheme)
    {
        bg = "#f5f2ee";
        fg = "#1a5070";
        yesBg = "#2a5080";
        yesHover = "#3668a0";
        noBg = "#8b4a4a";
        noHover = "#a55a5a";
        borderCol = "#d0ccc6";
    }
    else
    {
        bg = "#0a0e14";
        fg = "#a8c8f0";
        yesBg = "#2a5080";
        yesHover = "#3668a0";
        noBg = "#7a3a3a";
        noHover = "#944a4a";
        borderCol = "#1a2a44";
    }

    dlg.setStyleSheet(QString(
                          "QDialog { background: %1; border: 1px solid %7; border-radius: 12px; }"
                          "QLabel { color: %2; font-size: 14px; background: transparent; }"
                          "QLabel#dlgTitle { font-size: 18px; font-weight: bold; }")
                          .arg(bg, fg, yesBg, yesHover, noBg, noHover, borderCol));

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(30, 25, 30, 25);
    layout->setSpacing(16);

    QLabel *titleLbl = new QLabel(title);
    titleLbl->setObjectName("dlgTitle");
    titleLbl->setAlignment(Qt::AlignCenter);

    QLabel *textLbl = new QLabel(text);
    textLbl->setWordWrap(true);
    textLbl->setAlignment(Qt::AlignCenter);

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setSpacing(12);

    QPushButton *yesBtn = new QPushButton("Yes");
    yesBtn->setCursor(Qt::PointingHandCursor);
    yesBtn->setFixedHeight(40);
    yesBtn->setStyleSheet(QString(
                              "QPushButton { background: %1; color: white; border: none; border-radius: 8px;"
                              "  padding: 10px 24px; font-size: 14px; font-weight: bold; }"
                              "QPushButton:hover { background: %2; }")
                              .arg(yesBg, yesHover));

    QPushButton *noBtn = new QPushButton("No");
    noBtn->setCursor(Qt::PointingHandCursor);
    noBtn->setFixedHeight(40);
    noBtn->setStyleSheet(QString(
                             "QPushButton { background: %1; color: white; border: none; border-radius: 8px;"
                             "  padding: 10px 24px; font-size: 14px; font-weight: bold; }"
                             "QPushButton:hover { background: %2; }")
                             .arg(noBg, noHover));

    btnRow->addStretch();
    btnRow->addWidget(yesBtn);
    btnRow->addWidget(noBtn);
    btnRow->addStretch();

    connect(yesBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(noBtn, &QPushButton::clicked, &dlg, &QDialog::reject);

    layout->addWidget(titleLbl);
    layout->addWidget(textLbl);
    layout->addSpacing(4);
    layout->addLayout(btnRow);

    return dlg.exec() == QDialog::Accepted;
}

int DashboardPage::showStyledChoice(const QString &title, const QString &text,
                                    const QStringList &buttonLabels)
{
    QDialog dlg(this);
    dlg.setWindowTitle(title);
    dlg.setMinimumWidth(420);
    dlg.setMaximumWidth(520);

    QString bg, fg, btnBg, btnHover, borderCol;
    if (darkTheme)
    {
        bg = "#f5f2ee";
        fg = "#1a5070";
        btnBg = "#2a5080";
        btnHover = "#3668a0";
        borderCol = "#d0ccc6";
    }
    else
    {
        bg = "#0a0e14";
        fg = "#a8c8f0";
        btnBg = "#2a5080";
        btnHover = "#3668a0";
        borderCol = "#1a2a44";
    }

    dlg.setStyleSheet(QString(
                          "QDialog { background: %1; border: 1px solid %5; border-radius: 12px; }"
                          "QLabel { color: %2; font-size: 14px; background: transparent; }"
                          "QLabel#dlgTitle { font-size: 18px; font-weight: bold; }")
                          .arg(bg, fg, btnBg, btnHover, borderCol));

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(30, 25, 30, 25);
    layout->setSpacing(16);

    QLabel *titleLbl = new QLabel(title);
    titleLbl->setObjectName("dlgTitle");
    titleLbl->setAlignment(Qt::AlignCenter);

    QLabel *textLbl = new QLabel(text);
    textLbl->setWordWrap(true);
    textLbl->setAlignment(Qt::AlignCenter);

    QVBoxLayout *btnCol = new QVBoxLayout();
    btnCol->setSpacing(8);

    int result = -1;
    for (int i = 0; i < buttonLabels.size(); i++)
    {
        QPushButton *btn = new QPushButton(buttonLabels[i]);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(40);
        btn->setStyleSheet(QString(
                               "QPushButton { background: %1; color: white; border: none; border-radius: 8px;"
                               "  padding: 10px 20px; font-size: 13px; font-weight: bold; }"
                               "QPushButton:hover { background: %2; }")
                               .arg(btnBg, btnHover));
        connect(btn, &QPushButton::clicked, [&dlg, &result, i]()
                {
            result = i;
            dlg.accept(); });
        btnCol->addWidget(btn);
    }

    layout->addWidget(titleLbl);
    layout->addWidget(textLbl);
    layout->addSpacing(4);
    layout->addLayout(btnCol);

    dlg.exec();
    return result;
}

// =================== REFRESH METHODS ===================

void DashboardPage::refreshBrowse()
{
    browseTable->setRowCount(0);
    User *currentUser = fileManager->getCurrentUser();
    if (!currentUser)
        return;

    string query = searchEdit->text().trimmed().toStdString();
    vector<Item *> allItems = fileManager->getAllItems();

    for (Item *item : allItems)
    {
        if (!query.empty() && !item->matchesSearch(query))
            continue;
        if (item->getOwnerID() == currentUser->getID())
            continue;

        int row = browseTable->rowCount();
        browseTable->insertRow(row);

        browseTable->setItem(row, 0,
                             new QTableWidgetItem(QString::fromStdString(item->getName())));
        browseTable->setItem(row, 1,
                             new QTableWidgetItem(QString::fromStdString(item->getCategoryString())));

        QString desc = QString::fromStdString(item->getDescription());
        if (desc.length() > 50)
            desc = desc.left(47) + "...";
        browseTable->setItem(row, 2, new QTableWidgetItem(desc));

        User *owner = fileManager->findUserByStudentId(item->getOwnerID());
        browseTable->setItem(row, 3,
                             new QTableWidgetItem(owner
                                                      ? QString::fromStdString(owner->getFullName())
                                                      : "Unknown"));

        if (item->getAvailable())
        {
            QTableWidgetItem *statusItem = new QTableWidgetItem("Available");
            statusItem->setForeground(QBrush(QColor("#27ae60")));
            browseTable->setItem(row, 4, statusItem);
            browseTable->setItem(row, 5, new QTableWidgetItem("-"));

            QPushButton *requestBtn = new QPushButton("Request");
            string itemId = item->getID();
            connect(requestBtn, &QPushButton::clicked, [this, itemId]()
                    {
                User* cur = fileManager->getCurrentUser();
                if (!cur) return;

                bool ok;
                QString duration = QInputDialog::getText(this, "Request Item",
                    "Duration (e.g., '1 week', '3 days'):",
                    QLineEdit::Normal, "1 week", &ok);
                if (!ok || duration.isEmpty()) return;

                QString notes = QInputDialog::getText(this, "Request Item",
                    "Additional notes (optional):", QLineEdit::Normal, "", &ok);
                if (!ok) return;

                int reqId = fileManager->getNextRequestId();
                time_t now = time(0);
                tm* ltm = localtime(&now);
                char dateStr[20];
                snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d",
                         1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);

                Request newReq(to_string(reqId), cur->getID(), itemId,
                               duration.toStdString(), RequestStatus::PENDING,
                               string(dateStr), notes.toStdString());

                try {
                    fileManager->addRequest(newReq);
                    showStyledMsg("Success", "Request sent to item owner!");
                    refreshBrowse();
                } catch (const exception& e) {
                    showStyledMsg("Error", e.what());
                } });
            browseTable->setCellWidget(row, 6, requestBtn);
        }
        else
        {
            QTableWidgetItem *statusItem = new QTableWidgetItem("Borrowed");
            statusItem->setForeground(QBrush(QColor("#e74c3c")));
            browseTable->setItem(row, 4, statusItem);

            int wCount = fileManager->getWaitlistCount(item->getID());
            browseTable->setItem(row, 5,
                                 new QTableWidgetItem(QString("%1 waiting").arg(wCount)));

            bool alreadyWaiting = fileManager->isOnWaitlist(
                item->getID(), currentUser->getID());

            if (alreadyWaiting)
            {
                QLabel *waitLabel = new QLabel("  On Waitlist");
                waitLabel->setStyleSheet("color: #f39c12; font-weight: bold;");
                browseTable->setCellWidget(row, 6, waitLabel);
            }
            else
            {
                QPushButton *waitBtn = new QPushButton("Join Waitlist");
                waitBtn->setObjectName("warnBtn");
                string itemId = item->getID();
                connect(waitBtn, &QPushButton::clicked, [this, itemId]()
                        {
                    User* cur = fileManager->getCurrentUser();
                    if (!cur) return;
                    if (fileManager->addToWaitlist(itemId, cur->getID())) {
                        showStyledMsg("Waitlist",
                            "You've been added to the waitlist.\n"
                            "You'll get a request when the item becomes available.");
                        refreshBrowse();
                    } });
                browseTable->setCellWidget(row, 6, waitBtn);
            }
        }
    }
}

void DashboardPage::refreshMyItems()
{
    myItemsTable->clear();
    myItemsTable->setColumnCount(6);
    myItemsTable->setHorizontalHeaderLabels({"Name", "Category", "Description", "Status", "Listed", "Action"});
    myItemsTable->setRowCount(0);
    User *currentUser = fileManager->getCurrentUser();
    if (!currentUser)
        return;

    for (Item *item : fileManager->getAllItems())
    {
        if (item->getOwnerID() != currentUser->getID())
            continue;

        int row = myItemsTable->rowCount();
        myItemsTable->insertRow(row);

        myItemsTable->setItem(row, 0,
                              new QTableWidgetItem(QString::fromStdString(item->getName())));
        myItemsTable->setItem(row, 1,
                              new QTableWidgetItem(QString::fromStdString(item->getCategoryString())));
        myItemsTable->setItem(row, 2,
                              new QTableWidgetItem(QString::fromStdString(item->getDescription())));

        QTableWidgetItem *statusItem = new QTableWidgetItem(
            item->getAvailable() ? "Available" : "Borrowed");
        statusItem->setForeground(QBrush(
            item->getAvailable() ? QColor("#27ae60") : QColor("#e74c3c")));
        myItemsTable->setItem(row, 3, statusItem);

        myItemsTable->setItem(row, 4,
                              new QTableWidgetItem(QString::fromStdString(item->getDateListed())));

        // Action buttons
        QWidget *actionWidget = new QWidget();
        QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(0, 0, 0, 0);
        actionLayout->setSpacing(8);

        QPushButton *editBtn = new QPushButton("Edit");
        editBtn->setObjectName("successBtn");
        editBtn->setMinimumWidth(70);
        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setObjectName("dangerBtn");
        deleteBtn->setMinimumWidth(70);

        actionLayout->addWidget(editBtn);
        actionLayout->addWidget(deleteBtn);

        int itemId = 0;
        try
        {
            itemId = stoi(item->getID());
        }
        catch (...)
        {
            continue;
        }

        // Edit button logic
        connect(editBtn, &QPushButton::clicked, [this, itemId]()
                {
            Item* itm = fileManager->findItemById(itemId);
            if (!itm) { showStyledMsg("Error", "Item not found."); return; }
            bool ok;
            QString newName = QInputDialog::getText(this, "Edit Item Name", "Name:", QLineEdit::Normal, QString::fromStdString(itm->getName()), &ok);
            if (!ok) return;
            QString newDesc = QInputDialog::getText(this, "Edit Description", "Description:", QLineEdit::Normal, QString::fromStdString(itm->getDescription()), &ok);
            if (!ok) return;
            itm->setName(newName.toStdString());
            itm->setDescription(newDesc.toStdString());
            fileManager->saveAllDataToFiles();
            refreshMyItems();
            showStyledMsg("Success", "Item updated."); });

        // Delete button logic
        connect(deleteBtn, &QPushButton::clicked, [this, itemId]()
                {
            if (!showStyledConfirm("Delete Item", "Are you sure you want to delete this item?")) return;
            try { fileManager->removeItem(itemId); } catch (...) { showStyledMsg("Error", "Failed to delete item."); return; }
            fileManager->saveAllDataToFiles();
            refreshMyItems();
            showStyledMsg("Deleted", "Item removed."); });

        myItemsTable->setCellWidget(row, 5, actionWidget);
        myItemsTable->setRowHeight(row, 40);
    }
}

void DashboardPage::refreshRequests()
{
    requestsTable->setRowCount(0);
    User *currentUser = fileManager->getCurrentUser();
    if (!currentUser)
        return;

    int userId = 0;
    try
    {
        userId = stoi(currentUser->getID());
    }
    catch (...)
    {
        return;
    }

    // Rebuild headers (6 cols: Request# | From | Item | Duration | Notes | Action)
    requestsTable->clear();
    requestsTable->setColumnCount(6);
    requestsTable->setHorizontalHeaderLabels(
        {"Request#", "From", "Item", "Duration", "Notes", "Action"});
    requestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    requestsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    requestsTable->setColumnWidth(0, 75);
    requestsTable->setColumnWidth(1, 130);
    requestsTable->setColumnWidth(2, 130);
    requestsTable->setColumnWidth(3, 90);
    requestsTable->setColumnWidth(5, 240);

    vector<Request *> pending = fileManager->getPendingRequestsForUser(userId);

    for (Request *req : pending)
    {
        int row = requestsTable->rowCount();
        requestsTable->insertRow(row);

        // Col 0 — Request ID
        requestsTable->setItem(row, 0,
                               new QTableWidgetItem(QString::fromStdString(req->getRequestID())));

        // Col 1 — Borrower name
        User *borrower = fileManager->findUserByStudentId(req->getBorrowerID());
        requestsTable->setItem(row, 1,
                               new QTableWidgetItem(borrower
                                                        ? QString::fromStdString(borrower->getFullName())
                                                        : "Unknown"));

        // Col 2 — Item name
        Item *item = nullptr;
        try
        {
            item = fileManager->findItemById(stoi(req->getItemID()));
        }
        catch (...)
        {
        }
        requestsTable->setItem(row, 2,
                               new QTableWidgetItem(item
                                                        ? QString::fromStdString(item->getName())
                                                        : "Unknown"));

        // Col 3 — Duration
        requestsTable->setItem(row, 3,
                               new QTableWidgetItem(QString::fromStdString(req->getDuration())));

        // Col 4 — Notes (truncated)
        QString notesText = QString::fromStdString(req->getNotes());
        if (notesText.length() > 30)
            notesText = notesText.left(27) + "...";
        requestsTable->setItem(row, 4, new QTableWidgetItem(notesText));

        // Col 5 — Accept / Reject buttons (inline stylesheet so they render correctly)
        QWidget *actionWidget = new QWidget();
        QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(6, 6, 6, 6);
        actionLayout->setSpacing(8);

        QPushButton *acceptBtn = new QPushButton("Accept");
        acceptBtn->setFixedSize(104, 34);
        acceptBtn->setCursor(Qt::PointingHandCursor);
        acceptBtn->setStyleSheet(
            "QPushButton { background-color: #1a8a5a; color: white; border: none;"
            "  border-radius: 6px; font-size: 13px; font-weight: bold; }"
            "QPushButton:hover { background-color: #1e9460; }"
            "QPushButton:pressed { background-color: #157a4e; }");

        QPushButton *rejectBtn = new QPushButton("Reject");
        rejectBtn->setFixedSize(104, 34);
        rejectBtn->setCursor(Qt::PointingHandCursor);
        rejectBtn->setStyleSheet(
            "QPushButton { background-color: #c0392b; color: white; border: none;"
            "  border-radius: 6px; font-size: 13px; font-weight: bold; }"
            "QPushButton:hover { background-color: #e74c3c; }"
            "QPushButton:pressed { background-color: #a93226; }");

        actionLayout->addWidget(acceptBtn);
        actionLayout->addWidget(rejectBtn);
        actionLayout->addStretch();

        int reqId = 0;
        try
        {
            reqId = stoi(req->getRequestID());
        }
        catch (...)
        {
            continue;
        }

        connect(acceptBtn, &QPushButton::clicked, [this, reqId]()
                {
            try {
                if (fileManager->updateRequestStatus(reqId, "APPROVED")) {
                    Request *r = fileManager->findRequestById(reqId);
                    if (r) {
                        Item *itm = nullptr;
                        try { itm = fileManager->findItemById(stoi(r->getItemID())); } catch (...) {}
                        if (itm) {
                            itm->updateStatus(false);
                            fileManager->saveAllDataToFiles();
                        }
                    }
                    showStyledMsg("Accepted", "Request accepted! Connection created.");
                    refreshRequests();
                    refreshConnections();
                    refreshMyItems();
                    refreshBrowse();
                }
            } catch (const exception &e) {
                showStyledMsg("Error", e.what());
            } });

        connect(rejectBtn, &QPushButton::clicked, [this, reqId]()
                {
            if (showStyledConfirm("Reject", "Reject this request?")) {
                try {
                    fileManager->deleteRequest(reqId);
                    refreshRequests();
                } catch (const exception &e) {
                    showStyledMsg("Error", e.what());
                }
            } });

        requestsTable->setCellWidget(row, 5, actionWidget);
        requestsTable->setRowHeight(row, 48);
    }
}

void DashboardPage::refreshConnections()
{
    connectionsTable->setRowCount(0);
    User *currentUser = fileManager->getCurrentUser();
    if (!currentUser)
        return;

    int userId = 0;
    try
    {
        userId = stoi(currentUser->getID());
    }
    catch (...)
    {
        return;
    }

    vector<Request *> connections = fileManager->getApprovedRequestsForUser(userId);

    for (Request *req : connections)
    {
        Item *item = nullptr;
        try
        {
            item = fileManager->findItemById(stoi(req->getItemID()));
        }
        catch (...)
        {
        }
        if (!item)
            continue;

        User *lender = fileManager->findUserByStudentId(item->getOwnerID());
        User *borrower = fileManager->findUserByStudentId(req->getBorrowerID());
        if (!lender || !borrower)
            continue;

        bool isLender = (currentUser->getID() == item->getOwnerID());
        User *otherParty = isLender ? borrower : lender;

        int row = connectionsTable->rowCount();
        connectionsTable->insertRow(row);

        connectionsTable->setItem(row, 0,
                                  new QTableWidgetItem(QString::fromStdString(req->getRequestID())));
        connectionsTable->setItem(row, 1,
                                  new QTableWidgetItem(QString::fromStdString(item->getName())));
        connectionsTable->setItem(row, 2,
                                  new QTableWidgetItem(isLender ? "Lender" : "Borrower"));
        connectionsTable->setItem(row, 3,
                                  new QTableWidgetItem(QString::fromStdString(otherParty->getFullName())));
        connectionsTable->setItem(row, 4,
                                  new QTableWidgetItem(QString::fromStdString(otherParty->getContact())));
        connectionsTable->setItem(row, 5,
                                  new QTableWidgetItem(QString::fromStdString(
                                      otherParty->getEmail().empty() ? "N/A" : otherParty->getEmail())));

        if (isLender)
        {
            QPushButton *gotBackBtn = new QPushButton("Got Back");
            gotBackBtn->setMinimumWidth(100);
            string reqIdStr = req->getRequestID();
            string itemIdStr = item->getID();
            connect(gotBackBtn, &QPushButton::clicked, [this, reqIdStr, itemIdStr]()
                    {
                int choice = showStyledChoice("Item Return",
                    "Has the item been returned?",
                    {"Yes - Make available", "Yes - Remove item", "Cancel"});
                if (choice < 0 || choice == 2) return;

                int rId = 0;
                try { rId = stoi(reqIdStr); } catch (...) { return; }

                Item* itm = nullptr;
                try { itm = fileManager->findItemById(stoi(itemIdStr)); } catch (...) {}

                Request* r = fileManager->findRequestById(rId);
                if (!r) return;

                User* lndr = itm ? fileManager->findUserByStudentId(itm->getOwnerID()) : nullptr;
                User* brwr = fileManager->findUserByStudentId(r->getBorrowerID());

                if (lndr) lndr->incrementTransactions();
                if (brwr) brwr->incrementTransactions();

                if (choice == 0) {
                    if (itm) itm->updateStatus(true);
                    fileManager->deleteRequest(rId);

                    if (itm) {
                        string nextStudent = fileManager->promoteFromWaitlist(itm->getID());
                        if (!nextStudent.empty())//this is whwere the checking waitlist part happens, if there is a waitlist, it will auto create a request for the next person and notify them
                         {
                            int newReqId = fileManager->getNextRequestId();
                            time_t now = time(0);
                            tm* ltm = localtime(&now);
                            char dateStr[20];
                            snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d",
                                     1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);

                            Request autoReq(to_string(newReqId), nextStudent,
                                itm->getID(), "1 week", RequestStatus::PENDING,
                                string(dateStr), "Auto-created from waitlist");
                            fileManager->addRequest(autoReq);

                            User* wu = fileManager->findUserByStudentId(nextStudent);
                            showStyledMsg("Waitlist",
                                QString("Waitlisted user %1 has been auto-notified!")
                                .arg(wu ? QString::fromStdString(wu->getFullName())
                                        : QString::fromStdString(nextStudent)));
                        }
                    }
                    showStyledMsg("Success", "Item returned and marked available.");
                } else {
                    if (itm) {
                        try { fileManager->removeItem(stoi(itm->getID())); } catch (...) {}
                    }
                    fileManager->deleteRequest(rId);
                    showStyledMsg("Success", "Item removed from system.");
                }

                fileManager->saveAllDataToFiles();
                refreshAll(); });
            connectionsTable->setCellWidget(row, 6, gotBackBtn);
        }
        else
        {
            connectionsTable->setItem(row, 6, new QTableWidgetItem("(Borrower)"));
        }
    }
}

void DashboardPage::refreshProfile()
{
    User *user = fileManager->getCurrentUser();
    if (!user)
        return;

    Student *student = dynamic_cast<Student *>(user);

    QString info;
    info += QString("<b>Name:</b> %1<br>").arg(QString::fromStdString(user->getFullName()));
    info += QString("<b>ID:</b> %1<br>").arg(QString::fromStdString(user->getID()));
    info += QString("<b>Department:</b> %1<br>").arg(QString::fromStdString(user->getDepartment()));
    if (student)
    {
        info += QString("<b>Year:</b> %1<br>").arg(student->getYear());
        info += QString("<b>Verified:</b> %1<br>")
                    .arg(student->getVerified() ? "Yes (Email provided)" : "No");
    }
    info += QString("<b>Contact:</b> %1<br>").arg(QString::fromStdString(user->getContact()));
    info += QString("<b>Email:</b> %1<br>")
                .arg(QString::fromStdString(user->getEmail().empty() ? "N/A" : user->getEmail()));
    info += QString("<b>Total Transactions:</b> %1").arg(user->getTotalTransactions());

    profileInfoLabel->setTextFormat(Qt::RichText);
    profileInfoLabel->setText(info);
}

void DashboardPage::checkWaitlistNotifications()
{
    User *user = fileManager->getCurrentUser();
    if (!user)
        return;//if there is no user, return

    vector<Request *> allReqs = fileManager->getAllRequests();
    for (Request *req : allReqs)//goes thru all requests to find any pending requests that were auto-created from the waitlist for this user
    {
        if (req->getBorrowerID() == user->getID() &&
            req->isPending() &&
            req->getNotes().find("Auto-created from waitlist") != string::npos)//checks for any pending requests that were auto-created from the waitlist for this user
        {
            Item *item = nullptr;
            try
            {
                item = fileManager->findItemById(stoi(req->getItemID()));
            }
            catch (...){}
            QString itemName = item
                                   ? QString::fromStdString(item->getName())
                                   : "an item";
            showStyledMsg("Waitlist Notification",
                          QString("Good news! '%1' you were waiting for is now available.\n"
                                  "A pending request has been created for you!")
                              .arg(itemName));
            break;
        }
    }
}

// =================== ADD ITEM DIALOG ===================

void DashboardPage::onAddItem()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Item");
    dialog.setMinimumWidth(400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Item name");

    QLineEdit *descEdit = new QLineEdit();
    descEdit->setPlaceholderText("Description");

    QComboBox *catCombo = new QComboBox();
    catCombo->addItems({"TEXTBOOK", "ELECTRONICS", "CLOTHING",
                        "LAB_EQUIPMENT", "STATIONARY", "FOOD", "OTHER"});

    QPushButton *submitBtn = new QPushButton("Add Item");
    submitBtn->setObjectName("successBtn");

    QLabel *resultLabel = new QLabel("");

    layout->addWidget(new QLabel("Name:"));
    layout->addWidget(nameEdit);
    layout->addWidget(new QLabel("Description:"));
    layout->addWidget(descEdit);
    layout->addWidget(new QLabel("Category:"));
    layout->addWidget(catCombo);
    layout->addSpacing(10);
    layout->addWidget(submitBtn);
    layout->addWidget(resultLabel);

    FileManager *fm = fileManager;
    connect(submitBtn, &QPushButton::clicked, [&, fm]()
            {
        string name = nameEdit->text().trimmed().toStdString();
        string desc = descEdit->text().trimmed().toStdString();
        string catStr = catCombo->currentText().toStdString();

        if (name.empty() || desc.empty()) {
            resultLabel->setStyleSheet("color: #e74c3c;");
            resultLabel->setText("Name and description are required.");
            return;
        }

        User* user = fm->getCurrentUser();
        if (!user) return;

        int itemId = fm->getNextItemId();
        Category cat = Item::stringToCategory(catStr);

        Item newItem(to_string(itemId), name, cat, desc, true, user->getID());

        try {
            fm->addItem(newItem);
            resultLabel->setStyleSheet("color: #27ae60;");
            resultLabel->setText(QString("Item added! ID: %1").arg(itemId));
            submitBtn->setEnabled(false);
            refreshMyItems();
            refreshBrowse();
        } catch (const exception& e) {
            resultLabel->setStyleSheet("color: #e74c3c;");
            resultLabel->setText(QString("Error: %1").arg(e.what()));
        } });

    dialog.exec();
}

// =================== MAIN ===================

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}

#include "main_qt.moc"