class Btn
{
public:
    int x, y, w, h;
    char *label;
    Btn(){}
    Btn(int x, int y, int w, int h, char *label)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->label = label;
    }
};