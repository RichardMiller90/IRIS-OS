#pragma once

#include <QObject>
#include <QWaylandCompositor>
#include <QWaylandSurface>
#include <QWaylandSeat>
#include <QWaylandKeyboard>
#include <QTimer>
#include <QVector>
#include <QMap>
#include <QQmlEngine>
#include <QJSEngine>

class MobileCompositor;

struct KeyDef {
    QString label;
    QString shiftLabel;
    QString altLabel;
    int keyCode;
    Qt::Key qtKey;
    qreal widthWeight = 1.0;
    bool isModifier = false;
    bool isRepeatable = false;
};

class VirtualKeyboard : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(qreal keyboardHeight READ keyboardHeight NOTIFY keyboardHeightChanged)
    Q_PROPERTY(QString currentLayout READ currentLayout WRITE setCurrentLayout NOTIFY currentLayoutChanged)
    Q_PROPERTY(bool shiftActive READ shiftActive NOTIFY shiftActiveChanged)
    Q_PROPERTY(bool symbolMode READ symbolMode NOTIFY symbolModeChanged)
    Q_PROPERTY(QString previewText READ previewText NOTIFY previewTextChanged)
    Q_PROPERTY(QColor keyboardBackground READ keyboardBackground NOTIFY themeChanged)
    Q_PROPERTY(QColor keyBackground READ keyBackground NOTIFY themeChanged)
    Q_PROPERTY(QColor keyActiveBackground READ keyActiveBackground NOTIFY themeChanged)
    Q_PROPERTY(QColor keyTextColor READ keyTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor specialKeyBackground READ specialKeyBackground NOTIFY themeChanged)
    Q_PROPERTY(bool darkMode READ darkMode WRITE setDarkMode NOTIFY themeChanged)

public:
    enum LayoutType {
        Letters,
        Symbols,
        SymbolsExtended,
        Emoji
    };
    Q_ENUM(LayoutType)

    explicit VirtualKeyboard(MobileCompositor *compositor = nullptr);
    static VirtualKeyboard* create(QQmlEngine *engine, QJSEngine *scriptEngine);

    bool visible() const;
    qreal keyboardHeight() const;
    QString currentLayout() const;
    bool shiftActive() const;
    bool symbolMode() const;
    QString previewText() const;

    QColor keyboardBackground() const;
    QColor keyBackground() const;
    QColor keyActiveBackground() const;
    QColor keyTextColor() const;
    QColor specialKeyBackground() const;
    bool darkMode() const;

    Q_INVOKABLE QVariantList rowData(int row) const;
    Q_INVOKABLE int rowCount() const;
    Q_INVOKABLE void keyPressed(const QString &label, int keyCode, bool isModifier);
    Q_INVOKABLE void keyReleased(const QString &label, int keyCode);
    Q_INVOKABLE void keyCancelled();
    Q_INVOKABLE void toggleShift();
    Q_INVOKABLE void toggleSymbols();
    Q_INVOKABLE void handleBackspace();
    Q_INVOKABLE void handleReturn();
    Q_INVOKABLE void handleSpace();
    Q_INVOKABLE void handleGlobe();
    Q_INVOKABLE void handleDictation();
    Q_INVOKABLE void handleEmoji();

    void setTargetSurface(QWaylandSurface *surface);
    void showForSurface(QWaylandSurface *surface);
    void hide();

public slots:
    void setVisible(bool visible);
    void setCurrentLayout(const QString &layout);
    void setDarkMode(bool dark);

signals:
    void visibleChanged();
    void keyboardHeightChanged();
    void currentLayoutChanged();
    void shiftActiveChanged();
    void symbolModeChanged();
    void previewTextChanged();
    void themeChanged();
    void keyPressVisual(const QString &label, int row, int col);
    void keyReleaseVisual(int row, int col);
    void playHaptic();

private:
    void buildLayout();
    void sendKeyEvent(int keyCode, bool pressed);
    void sendText(const QString &text);
    void updatePreview();
    QString applyShift(const QString &text);

    MobileCompositor *m_compositor = nullptr;
    QWaylandSurface *m_targetSurface = nullptr;
    QWaylandSeat *m_seat = nullptr;

    bool m_visible = false;
    bool m_shiftActive = false;
    bool m_capsLock = false;
    bool m_symbolMode = false;
    bool m_darkMode = true;
    QString m_currentLayout = "en";
    QString m_previewText;

    QVector<QVector<KeyDef>> m_layout;
    QTimer *m_backspaceTimer = nullptr;
    QTimer *m_shiftDoubleTapTimer = nullptr;

    static constexpr qreal KEY_HEIGHT_DP = 54;
    static constexpr qreal KEY_SPACING_DP = 6;
    static constexpr qreal BOTTOM_PADDING_DP = 34;

    // Linux input event codes
    static constexpr int KEY_ESC = 1;
    static constexpr int KEY_1 = 2;
    static constexpr int KEY_2 = 3;
    static constexpr int KEY_3 = 4;
    static constexpr int KEY_4 = 5;
    static constexpr int KEY_5 = 6;
    static constexpr int KEY_6 = 7;
    static constexpr int KEY_7 = 8;
    static constexpr int KEY_8 = 9;
    static constexpr int KEY_9 = 10;
    static constexpr int KEY_0 = 11;
    static constexpr int KEY_MINUS = 12;
    static constexpr int KEY_EQUAL = 13;
    static constexpr int KEY_BACKSPACE = 14;
    static constexpr int KEY_TAB = 15;
    static constexpr int KEY_Q = 16;
    static constexpr int KEY_W = 17;
    static constexpr int KEY_E = 18;
    static constexpr int KEY_R = 19;
    static constexpr int KEY_T = 20;
    static constexpr int KEY_Y = 21;
    static constexpr int KEY_U = 22;
    static constexpr int KEY_I = 23;
    static constexpr int KEY_O = 24;
    static constexpr int KEY_P = 25;
    static constexpr int KEY_LEFTBRACE = 26;
    static constexpr int KEY_RIGHTBRACE = 27;
    static constexpr int KEY_ENTER = 28;
    static constexpr int KEY_LEFTCTRL = 29;
    static constexpr int KEY_A = 30;
    static constexpr int KEY_S = 31;
    static constexpr int KEY_D = 32;
    static constexpr int KEY_F = 33;
    static constexpr int KEY_G = 34;
    static constexpr int KEY_H = 35;
    static constexpr int KEY_J = 36;
    static constexpr int KEY_K = 37;
    static constexpr int KEY_L = 38;
    static constexpr int KEY_SEMICOLON = 39;
    static constexpr int KEY_APOSTROPHE = 40;
    static constexpr int KEY_GRAVE = 41;
    static constexpr int KEY_LEFTSHIFT = 42;
    static constexpr int KEY_BACKSLASH = 43;
    static constexpr int KEY_Z = 44;
    static constexpr int KEY_X = 45;
    static constexpr int KEY_C = 46;
    static constexpr int KEY_V = 47;
    static constexpr int KEY_B = 48;
    static constexpr int KEY_N = 49;
    static constexpr int KEY_M = 50;
    static constexpr int KEY_COMMA = 51;
    static constexpr int KEY_DOT = 52;
    static constexpr int KEY_SLASH = 53;
    static constexpr int KEY_RIGHTSHIFT = 54;
    static constexpr int KEY_KPASTERISK = 55;
    static constexpr int KEY_LEFTALT = 56;
    static constexpr int KEY_SPACE = 57;
    static constexpr int KEY_CAPSLOCK = 58;
    static constexpr int KEY_F1 = 59;
    static constexpr int KEY_F2 = 60;
    static constexpr int KEY_F3 = 61;
    static constexpr int KEY_F4 = 62;
    static constexpr int KEY_F5 = 63;
    static constexpr int KEY_F6 = 64;
    static constexpr int KEY_F7 = 65;
    static constexpr int KEY_F8 = 66;
    static constexpr int KEY_F9 = 67;
    static constexpr int KEY_F10 = 68;
    static constexpr int KEY_NUMLOCK = 69;
    static constexpr int KEY_SCROLLLOCK = 70;
    static constexpr int KEY_KP7 = 71;
    static constexpr int KEY_KP8 = 72;
    static constexpr int KEY_KP9 = 73;
    static constexpr int KEY_KPMINUS = 74;
    static constexpr int KEY_KP4 = 75;
    static constexpr int KEY_KP5 = 76;
    static constexpr int KEY_KP6 = 77;
    static constexpr int KEY_KPPLUS = 78;
    static constexpr int KEY_KP1 = 79;
    static constexpr int KEY_KP2 = 80;
    static constexpr int KEY_KP3 = 81;
    static constexpr int KEY_KP0 = 82;
    static constexpr int KEY_KPDOT = 83;
    static constexpr int KEY_ZENKAKUHANKAKU = 85;
    static constexpr int KEY_102ND = 86;
    static constexpr int KEY_F11 = 87;
    static constexpr int KEY_F12 = 88;
    static constexpr int KEY_RO = 89;
    static constexpr int KEY_KATAKANA = 90;
    static constexpr int KEY_HIRAGANA = 91;
    static constexpr int KEY_HENKAN = 92;
    static constexpr int KEY_KATAKANAHIRAGANA = 93;
    static constexpr int KEY_MUHENKAN = 94;
    static constexpr int KEY_KPJPCOMMA = 95;
    static constexpr int KEY_KPENTER = 96;
    static constexpr int KEY_RIGHTCTRL = 97;
    static constexpr int KEY_KPSLASH = 98;
    static constexpr int KEY_SYSRQ = 99;
    static constexpr int KEY_RIGHTALT = 100;
    static constexpr int KEY_LINEFEED = 101;
    static constexpr int KEY_HOME = 102;
    static constexpr int KEY_UP = 103;
    static constexpr int KEY_PAGEUP = 104;
    static constexpr int KEY_LEFT = 105;
    static constexpr int KEY_RIGHT = 106;
    static constexpr int KEY_END = 107;
    static constexpr int KEY_DOWN = 108;
    static constexpr int KEY_PAGEDOWN = 109;
    static constexpr int KEY_INSERT = 110;
    static constexpr int KEY_DELETE = 111;
    static constexpr int KEY_MACRO = 112;
    static constexpr int KEY_MUTE = 113;
    static constexpr int KEY_VOLUMEDOWN = 114;
    static constexpr int KEY_VOLUMEUP = 115;
    static constexpr int KEY_POWER = 116;
    static constexpr int KEY_KPEQUAL = 117;
    static constexpr int KEY_KPPLUSMINUS = 118;
    static constexpr int KEY_PAUSE = 119;
    static constexpr int KEY_SCALE = 120;
    static constexpr int KEY_KPCOMMA = 121;
    static constexpr int KEY_HANGEUL = 122;
    static constexpr int KEY_HANGUEL = KEY_HANGEUL;
    static constexpr int KEY_HANJA = 123;
    static constexpr int KEY_YEN = 124;
    static constexpr int KEY_LEFTMETA = 125;
    static constexpr int KEY_RIGHTMETA = 126;
    static constexpr int KEY_COMPOSE = 127;
    static constexpr int KEY_STOP = 128;
    static constexpr int KEY_AGAIN = 129;
    static constexpr int KEY_PROPS = 130;
    static constexpr int KEY_UNDO = 131;
    static constexpr int KEY_FRONT = 132;
    static constexpr int KEY_COPY = 133;
    static constexpr int KEY_OPEN = 134;
    static constexpr int KEY_PASTE = 135;
    static constexpr int KEY_FIND = 136;
    static constexpr int KEY_CUT = 137;
    static constexpr int KEY_HELP = 138;
    static constexpr int KEY_MENU = 139;
    static constexpr int KEY_CALC = 140;
    static constexpr int KEY_SETUP = 141;
    static constexpr int KEY_SLEEP = 142;
    static constexpr int KEY_WAKEUP = 143;
    static constexpr int KEY_FILE = 144;
    static constexpr int KEY_SENDFILE = 145;
    static constexpr int KEY_DELETEFILE = 146;
    static constexpr int KEY_XFER = 147;
    static constexpr int KEY_PROG1 = 148;
    static constexpr int KEY_PROG2 = 149;
    static constexpr int KEY_WWW = 150;
    static constexpr int KEY_MSDOS = 151;
    static constexpr int KEY_COFFEE = 152;
    static constexpr int KEY_SCREENLOCK = KEY_COFFEE;
    static constexpr int KEY_ROTATE_DISPLAY = 153;
    static constexpr int KEY_DIRECTION = KEY_ROTATE_DISPLAY;
    static constexpr int KEY_CYCLEWINDOWS = 154;
    static constexpr int KEY_MAIL = 155;
    static constexpr int KEY_BOOKMARKS = 156;
    static constexpr int KEY_COMPUTER = 157;
    static constexpr int KEY_BACK = 158;
    static constexpr int KEY_FORWARD = 159;
    static constexpr int KEY_CLOSECD = 160;
    static constexpr int KEY_EJECTCD = 161;
    static constexpr int KEY_EJECTCLOSECD = 162;
    static constexpr int KEY_NEXTSONG = 163;
    static constexpr int KEY_PLAYPAUSE = 164;
    static constexpr int KEY_PREVIOUSSONG = 165;
    static constexpr int KEY_STOPCD = 166;
    static constexpr int KEY_RECORD = 167;
    static constexpr int KEY_REWIND = 168;
    static constexpr int KEY_PHONE = 169;
    static constexpr int KEY_ISO = 170;
    static constexpr int KEY_CONFIG = 171;
    static constexpr int KEY_HOMEPAGE = 172;
    static constexpr int KEY_REFRESH = 173;
    static constexpr int KEY_EXIT = 174;
    static constexpr int KEY_MOVE = 175;
    static constexpr int KEY_EDIT = 176;
    static constexpr int KEY_SCROLLUP = 177;
    static constexpr int KEY_SCROLLDOWN = 178;
    static constexpr int KEY_KPLEFTPAREN = 179;
    static constexpr int KEY_KPRIGHTPAREN = 180;
    static constexpr int KEY_NEW = 181;
    static constexpr int KEY_REDO = 182;
    static constexpr int KEY_F13 = 183;
    static constexpr int KEY_F14 = 184;
    static constexpr int KEY_F15 = 185;
    static constexpr int KEY_F16 = 186;
    static constexpr int KEY_F17 = 187;
    static constexpr int KEY_F18 = 188;
    static constexpr int KEY_F19 = 189;
    static constexpr int KEY_F20 = 190;
    static constexpr int KEY_F21 = 191;
    static constexpr int KEY_F22 = 192;
    static constexpr int KEY_F23 = 193;
    static constexpr int KEY_F24 = 194;
    static constexpr int KEY_MICMUTE = 248;
};
