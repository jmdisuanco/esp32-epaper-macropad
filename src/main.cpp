#include <M5EPD.h>
#include <BleCombo.h>
#include <Btn.h>
BleComboKeyboard Keyboard("JM's Controller", "JMart", 100);
BleComboMouse Mouse(&Keyboard);
M5EPD_Canvas canvas(&M5.EPD);

int point[2][2];
int past[2][2];
bool first = true;

int ROTATION = 0;
int SCREEN_WIDTH = 960;
int SCREEN_HEIGHT = 540;
const int Column = 3;
int touchX = 0;
int touchY = 0;
const int buttonH = 100;
const int buttonW = 100;
int touchH = SCREEN_HEIGHT;
int touchW = SCREEN_WIDTH - Column * buttonW;

// Define buttons
char *
    ShortCutButtons[15] = {
        "Play", "Mute", "Lock", "Esc", "Left",
        "<<", "V+", "=|", "Up", "Down",
        ">>", "V-", "|=", "Enter", "Right"};

int buttonX = touchW;
int buttonY = SCREEN_HEIGHT - buttonH * (sizeof(ShortCutButtons) / sizeof(char *) / Column);

Btn MacroPadMatrix[Column][sizeof(ShortCutButtons) / sizeof(char *) / Column];

void generateMacropadMatrix()
{
    int x = 0;
    for (int i = 0; i < Column; i++)
    {
        for (int j = 0; j < sizeof(ShortCutButtons) / sizeof(char *) / Column; j++)
        {
            MacroPadMatrix[i][j] = Btn(buttonX + buttonW * i,
                                       buttonY + buttonH * j,
                                       buttonW,
                                       buttonH,
                                       ShortCutButtons[x++]);
        }
    }
}

void drawTrackPad(int x, int y, int w, int h)
{
    canvas.drawRect(x + 2, y + 2, w - 4, h - 4, 15);
    canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
    // Serial.printf("Drawn trackpad at %d, %d, %d, %d \n", x, y, w, h);
}

void drawButton(int x, int y, int w, int h, String label)
{
    canvas.drawCentreString(label, x + 50, y + (h / 2) - 5, 1);
    canvas.drawRoundRect(x, y, w, h, 2, 15);
    // Serial.printf("Button: %d, %d, %d, %d", x, y, w, h);
}

void drawMacroPadMatrix(Btn MacroPadMatrix[Column][sizeof(ShortCutButtons) / sizeof(char *) / Column])
{
    for (int i = 0; i < Column; i++)
    {
        for (int j = 0; j < sizeof(ShortCutButtons) / sizeof(char *) / Column; j++)
        {
            drawButton(MacroPadMatrix[i][j].x, MacroPadMatrix[i][j].y, MacroPadMatrix[i][j].w, MacroPadMatrix[i][j].h, MacroPadMatrix[i][j].label);
        }
    }
}

void drawScreen()
{
    M5.EPD.SetRotation(ROTATION);
    M5.TP.SetRotation(ROTATION);
    Serial.printf("Screen Width: %d, Screen Height: %d, Rotation %d \n", SCREEN_WIDTH, SCREEN_HEIGHT, ROTATION);
    M5.EPD.Clear(true);
    canvas.createCanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    drawTrackPad(0, 0, touchW, touchH);
    canvas.setTextSize(2);
    canvas.setTextColor(WHITE);
    canvas.drawString("JM's Controller!", 45, SCREEN_HEIGHT / 2 - 5);

    drawMacroPadMatrix(MacroPadMatrix);
    canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
}

bool isPressed(int inputX, int inputY, int x, int y, int w, int h)
{
    if (inputX > x && inputX < x + w && inputY > y && inputY < y + h)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MatrixInputListener(int inputX, int inputY, Btn MacroPadMatrix[Column][sizeof(ShortCutButtons) / sizeof(char *) / Column])
{
    for (int i = 0; i < Column; i++)
    {
        for (int j = 0; j < sizeof(ShortCutButtons) / sizeof(char *) / Column; j++)
        {
            if (isPressed(inputX, inputY, MacroPadMatrix[i][j].x, MacroPadMatrix[i][j].y, MacroPadMatrix[i][j].w, MacroPadMatrix[i][j].h))
            {
                Serial.println(MacroPadMatrix[i][j].label);
                if (MacroPadMatrix[i][j].label == "Up")
                {
                    Keyboard.press(KEY_UP_ARROW);
                }
                else if (MacroPadMatrix[i][j].label == "Down")
                {
                    Keyboard.press(KEY_DOWN_ARROW);
                }
                else if (MacroPadMatrix[i][j].label == "Left")
                {
                    Keyboard.press(KEY_LEFT_ARROW);
                }
                else if (MacroPadMatrix[i][j].label == "Right")
                {
                    Keyboard.press(KEY_RIGHT_ARROW);
                }
                else if (MacroPadMatrix[i][j].label == "Enter")
                {
                    Keyboard.press(KEY_RETURN);
                }
                else if (MacroPadMatrix[i][j].label == "V+")
                {
                    Keyboard.press(KEY_MEDIA_VOLUME_UP);
                    Keyboard.release(KEY_MEDIA_VOLUME_UP);
                }
                else if (MacroPadMatrix[i][j].label == "V-")
                {
                    Keyboard.press(KEY_MEDIA_VOLUME_DOWN);
                    Keyboard.release(KEY_MEDIA_VOLUME_DOWN);
                }
                else if (MacroPadMatrix[i][j].label == "Mute")
                {
                    Keyboard.press(KEY_MEDIA_MUTE);
                }
                else if (MacroPadMatrix[i][j].label == "Lock")
                {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_RIGHT_GUI);
                    Keyboard.press('q');
                    delay(300);
                }
                else if (MacroPadMatrix[i][j].label == "Esc")
                {
                    Keyboard.press(KEY_ESC);
                }
                else if (MacroPadMatrix[i][j].label == ">>")
                {
                    Keyboard.press(KEY_MEDIA_NEXT_TRACK);
                }
                else if (MacroPadMatrix[i][j].label == "<<")
                {
                    Keyboard.press(KEY_MEDIA_PREVIOUS_TRACK);
                }
                else if (MacroPadMatrix[i][j].label == "=|")
                {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.press(KEY_LEFT_ARROW);
                }
                else if (MacroPadMatrix[i][j].label == "|=")
                {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.press(KEY_RIGHT_ARROW);
                }
                else if (MacroPadMatrix[i][j].label == "|=")
                {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.press(KEY_RIGHT_ARROW);
                }
                else if (MacroPadMatrix[i][j].label == "BRB")
                {
                    Keyboard.println("Be right back!");
                }

                delay(100);
                Keyboard.releaseAll();
            }
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting...");
    M5.begin();
    generateMacropadMatrix();
    // M5.SHT30.Begin();
    // M5.RTC.begin();
    delay(300);
    Keyboard.begin();
    Mouse.begin();
    drawScreen();
}

void loop()
{
    if (M5.TP.avaliable())
    {
        if (!M5.TP.isFingerUp())
        {
            M5.TP.update();
            for (int i = 0; i < 2; i++)
            {
                tp_finger_t FingerItem = M5.TP.readFinger(i);
                if ((point[i][0] != FingerItem.x) || (point[i][1] != FingerItem.y))
                {
                    point[i][0] = FingerItem.x;
                    point[i][1] = FingerItem.y;

                    if (first)
                    {
                        past[i][0] = FingerItem.x;
                        past[i][1] = FingerItem.y;

                        if (FingerItem.size > 20)
                        {
                            FingerItem.id == 0 ? Mouse.click(MOUSE_LEFT) : Mouse.click(MOUSE_RIGHT);
                        }
                        MatrixInputListener(FingerItem.x, FingerItem.y, MacroPadMatrix);
                    }

                    Serial.printf("Finger ID:%d--> X: %d  Y: %d  Size: %d\r\n", FingerItem.id, FingerItem.x, FingerItem.y, FingerItem.size);

                    // Trackpad
                    // Serial.printf(" %d , %d", point[0][0] - past[0][0], point[0][1] - past[0][1]);
                    if (point[0][0] > touchX && point[0][0] < touchX + touchW && point[0][1] > touchY && point[0][1] < touchY + touchH)
                    {
                        Mouse.move(point[0][0] - past[0][0], point[0][1] - past[0][1]);
                    }
                    first = false;
                    past[i][0] = point[i][0];
                    past[i][1] = point[i][1];
                }
            }
        }
        else
        {
            first = true;
        }
    }
    // M5.BtnL.lastChange();
    // M5.update();
    vTaskDelay(1 / portTICK_PERIOD_MS);
}
