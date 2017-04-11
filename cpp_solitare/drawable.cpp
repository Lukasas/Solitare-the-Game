#include "drawable.h"

drawable::drawable() : bHighlight(false)
{
    this->setPalette(palette);
}

QPixmap *drawable::GetImage() const
{
    return image;
}

void drawable::LoadQPixmap(QString path)
{
    image = new QPixmap();
    image->load("Resources/" + path);
}

void drawable::SetQPixmap(QPixmap *pixMap)
{
    image = new QPixmap(pixMap);

}

drawable::~drawable()
{
    if (image)
    {
        delete image;
    }
}

void drawable::Draw()
{
    // Will it work ? Should be
    palette.setBrush(QPalette::Background, image);
}

void drawable::Highlight(bool enable)
{
    // Highlight implementation
}

void drawable::ToggleHighlight()
{
    Highlight(!bHighlight);
}

bool drawable::IsHighlighted() const
{
    return bHighlight;
}
