////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "wxScrolledSFMLWindow.hpp"

// Platform-specific includes
#ifdef __WXGTK__
    #include <gdk/gdkx.h>
    #include <gtk/gtk.h>

#endif


////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(wxScrolledSFMLWindow, wxScrolledCanvas)
    EVT_ERASE_BACKGROUND(wxScrolledSFMLWindow::OnEraseBackground)
END_EVENT_TABLE()


////////////////////////////////////////////////////////////
/// Construct the wxScrolledSFMLWindow
////////////////////////////////////////////////////////////
wxScrolledSFMLWindow::wxScrolledSFMLWindow(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) :
wxScrolledCanvas(Parent, Id, Position, Size, Style)
{
    // Don't want double buffering SFML handles this for us
    SetDoubleBuffered(false);

    #ifdef __WXGTK__
        gtk_widget_realize(m_wxwindow);
        GtkWidget* widget = gtk_bin_get_child(GTK_BIN(GetHandle()));
        Display* display = GDK_WINDOW_XDISPLAY(widget->window);
        XFlush(display);
        sf::RenderWindow::create(GDK_WINDOW_XWINDOW(widget->window));
    #else
        // Tested under Windows XP only (should work with X11 and other Windows versions - no idea about MacOS)
        sf::RenderWindow::create(GetSystemHandle());
    #endif
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
wxScrolledSFMLWindow::~wxScrolledSFMLWindow()
{
    // Nothing to do...
}


////////////////////////////////////////////////////////////
/// Notification for the derived class that moment is good
/// for doing its update and drawing stuff
////////////////////////////////////////////////////////////
void wxScrolledSFMLWindow::onUpdate()
{

}

////////////////////////////////////////////////////////////
/// Called when the window is repainted - we can display our
/// SFML window
///
////////////////////////////////////////////////////////////
void wxScrolledSFMLWindow::OnDraw(wxDC &dc)
{
    // Set Active
    setActive(true);

    // Update the View
    updateScroll();

    // Let the derived class do its specific stuff
    onUpdate();

    // Display on screen
    display();
}

////////////////////////////////////////////////////////////
/// Called when the control needs to draw its background
////////////////////////////////////////////////////////////
void wxScrolledSFMLWindow::OnEraseBackground(wxEraseEvent& event)
{
    // Don't do anything. We intercept this event in order to prevent the
    // parent class to draw the background before repainting the window,
    // which would cause some flickering
}

////////////////////////////////////////////////////////////
/// Updates the view on our control override to change
/// behavior
///
////////////////////////////////////////////////////////////
void wxScrolledSFMLWindow::updateScroll()
{
    int sx, sy, sw, sh;
    CalcUnscrolledPosition(0, 0, &sx, &sy);
    GetClientSize(&sw, &sh);
    sf::View view = getView();
    view.reset(sf::FloatRect(sx, sy, sw, sh));
    setView(view);
}

////////////////////////////////////////////////////////////
/// Overriden since we don't want to physically scroll
/// the window contents just the contents our SFMLView
/// is displaying.
////////////////////////////////////////////////////////////
void wxScrolledSFMLWindow::ScrollWindow(int dx, int dy, const wxRect* rect)
{

}
