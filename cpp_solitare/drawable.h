#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QWidget>
#include <QPixmap>

/**
 * @brief The drawable class
 *          Parent class for drawing
 *
 * Serves for child classes, so they'll be able to load their texture
 */
class drawable : public QWidget
{
public:

    drawable();

    /**
     * @brief SetQPixmap
     *      Sets already loaded Pixmap so it can be used as a texture for current instance
     * @param pxMap
     *      Pointer to loaded Pixmap, using Copy Constructor for private image member
     */
    void SetQPixmap(QPixmap *pxMap);

    /**
     * @brief GetImage
     *      Returns current image
     * @return
     *      Returns image that has been set, NULL otherwise
     */
    QPixmap *GetImage() const;
    ~drawable();

    /**
     * @brief Draw
     * Overridable method that draws the component
     */
    virtual void Draw() = 0;

    /**
     * @brief Highlight
     *      Sets the current instance of this class to be visibly highlighted
     * @param enable
     *      Accepts true/false/maybe
     */
    void Highlight(bool enable);

    /**
     * @brief ToggleHighlight
     *      Flips the switch on Highlight
     */
    void ToggleHighlight();

    /**
     * @brief IsHighlighted
     *      Gets information about highlighting
     * @return
     *      Returns True/False/Don't Know
     */
    bool IsHighlighted() const;
private:
    /**
     * @brief image
     * Variable that holds the texture data
     */
    QPixmap *image;
    bool bHighlight;

protected:
    QPalette palette;

    /**
     * @brief LoadQPixmap
     *      Loads an image from given path
     * @param path
     *      Path to file that must be located in Resources folder example: images/img.png
     */
    virtual void LoadQPixmap(QString path) = 0;

};

#endif // DRAWABLE_H
