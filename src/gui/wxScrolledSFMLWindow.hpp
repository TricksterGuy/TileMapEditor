#ifndef wxScrolledSFMLWindow_HPP
#define wxScrolledSFMLWindow_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <wx/wx.h>
#include <wx/scrolwin.h>


////////////////////////////////////////////////////////////
/// wxSFMLWindow allows to run SFML in a wxWidgets control
////////////////////////////////////////////////////////////
class wxScrolledSFMLWindow : public wxScrolledCanvas, public sf::RenderWindow
{
    public :

        ////////////////////////////////////////////////////////////
        /// Construct the wxSFMLWindow
        ///
        /// \param Parent :   Parent of the control (NULL by default)
        /// \param Id :       Identifier of the control (-1 by default)
        /// \param Position : Position of the control (wxDefaultPosition by default)
        /// \param Size :     Size of the control (wxDefaultSize by default)
        /// \param Style :    Style of the control (0 by default)
        ///
        ////////////////////////////////////////////////////////////
        wxScrolledSFMLWindow(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);

        ////////////////////////////////////////////////////////////
        /// Destructor
        ///
        ////////////////////////////////////////////////////////////
        virtual ~wxScrolledSFMLWindow();

        ////////////////////////////////////////////////////////////
        /// Override to stop wxWidgets default scroll behavior
        ///
        ////////////////////////////////////////////////////////////
        virtual void ScrollWindow(int dx, int dy, const wxRect* rect = NULL);

    private :

        DECLARE_EVENT_TABLE()

        ////////////////////////////////////////////////////////////
        /// Notification for the derived class that moment is good
        /// for doing its update and drawing stuff
        ///
        ////////////////////////////////////////////////////////////
        virtual void onUpdate();

        ////////////////////////////////////////////////////////////
        /// Called when the window is repainted - we can display our
        /// SFML window
        ///
        ////////////////////////////////////////////////////////////
        void OnDraw (wxDC &dc);

        ////////////////////////////////////////////////////////////
        /// Called when the window needs to draw its background
        ///
        ////////////////////////////////////////////////////////////
        void OnEraseBackground(wxEraseEvent&);

        ////////////////////////////////////////////////////////////
        /// Updates the view on our control override to change
        /// behavior
        ///
        ////////////////////////////////////////////////////////////
        virtual void updateScroll();
};

#endif // wxScrolledSFMLWindow_HPP
