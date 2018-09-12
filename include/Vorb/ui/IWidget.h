//
// IWidget.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IWidget.h
* @brief 
* Abstract Container for widgets
*
*/

#pragma once

#ifndef Vorb_IContainer_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IContainer_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#ifdef VORB_USING_SCRIPT
#include "Vorb/script/Function.h"
#endif

#include <vector>

#include "Vorb/Events.hpp"
#include "Vorb/VorbPreDecl.inl"

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        // Forward Declarations
        struct MouseButtonEvent;
        struct MouseMotionEvent;
        struct MouseEvent;
        struct WindowResizeEvent;
        class Widget;
        class GameWindow;

        //! Enum of clipping states.
        enum class ClippingState {
            VISIBLE = 0,
            HIDDEN,
            INHERIT
        };
        //! Bitfield of clipping flags.
        struct Clipping {
            ClippingState left   : 2;
            ClippingState top    : 2;
            ClippingState right  : 2;
            ClippingState bottom : 2;
        };
        const Clipping DEFAULT_CLIPPING = {
            ClippingState::VISIBLE,
            ClippingState::VISIBLE,
            ClippingState::VISIBLE,
            ClippingState::VISIBLE
        };
        //! Bitfield of dock flags
        enum class DockStyle {
            NONE = 0, LEFT, RIGHT, BOTTOM, TOP, FILL
        };

        //! Bitfield of widget flags
        struct WidgetFlags {
            bool isClicking : 1; ///< Used for click event tracking.
            bool isEnabled  : 1; ///< True when events are enabled.
            bool isMouseIn  : 1; ///< Used for motion event tracking.

            volatile bool needsDimensionUpdate       : 1; ///< Whether we need to recalculate widget dimensions.
            volatile bool needsDockRecalculation     : 1; ///< Whether we need to recalculate docking of child widgets.
            volatile bool needsClipRectRecalculation : 1; ///< Whether we need to recalculate the clip rectangle.
            volatile bool needsDrawableRefresh       : 1; ///< Whether we need to refresh our drawables.
        };

        // Forward Declarations
        class UIRenderer;

        class IWidget {
            using IWidgets = std::vector<IWidget*>;
            using Font = vorb::graphics::SpriteFont;
        public:
            /*! @brief Constructor default and parametised to set renderer and window.
             * 
             * @param renderer: The renderer to use for drawing widget.
             * @param window: The window in which to set the UI.
             */
            IWidget(UIRenderer* renderer, const GameWindow* window = nullptr);
            // /*! @brief Constructor that sets name, position, and dimensions.
            //  *
            //  * @param name: Name of the widget.
            //  * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            //  */
            // IWidget(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~IWidget();
            /*! @brief Releases all resources used by the Widget.
             *
             * Gets called in the destructor.
             */
            virtual void dispose();

            /*! @brief Updates the widget. Can be used for animation.
            *
            * @param dt: The TimeStep
            */
            virtual void update(f32 dt VORB_MAYBE_UNUSED);

            /*! @brief Enables events that all widgets share in common. */
            virtual void enable();
            /*! @brief Disables events that all widgets share in common. */
            virtual void disable();

            /*! @brief Childs another widget to this widget.
             *
             * @param child: The Widget to add
             * @return true on success.
             */
            virtual bool addWidget(IWidget* child);
            /*! @brief Unchilds a child widget of this widget.
            *
            * @param child: The Widget to remove
            * @return true on success.
            */
            virtual bool removeWidget(IWidget* child);

            /*! @brief Checks if a point is inside the container
             *
             * @param point: The point to check
             * @return true if point is in the bounds of the container
             */
            virtual bool isInBounds(const f32v2& point) const { return isInBounds(point.x, point.y); }
            virtual bool isInBounds(f32 x, f32 y) const;

            // virtual void setChildDock(Widget* widget, DockStyle dockStyle);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual       UIRenderer* getRenderer()         const { return m_renderer; }
            virtual const GameWindow* getGameWindow()       const { return m_window ? m_window : m_canvas->m_window; }
            virtual          IWidget* getCanvas()           const { return m_canvas; }
            virtual          IWidget* getParent()           const { return m_parent; }
            virtual   const IWidgets& getWidgets()          const { return m_widgets; }
            virtual       const Font* getFont()             const { return m_font; }
            virtual             f32v4 getDestRect()         const { return f32v4(m_position.x, m_position.y, m_size.x, m_size.y); }
            virtual        const f32& getX()                const { return m_position.x; }
            virtual        const f32& getY()                const { return m_position.y; }
            virtual      const f32v2& getPosition()         const { return m_position; }
            virtual        const f32& getWidth()            const { return m_size.x; }
            virtual        const f32& getHeight()           const { return m_size.y; }
            virtual      const f32v2& getSize()             const { return m_size; }
            virtual          Clipping getClipping()         const { return m_clipping; }
            virtual     ClippingState getClippingLeft()     const;
            virtual     ClippingState getClippingTop()      const;
            virtual     ClippingState getClippingRight()    const;
            virtual     ClippingState getClippingBottom()   const;
            virtual             f32v4 getClipRect()         const { return m_clipRect; }
            virtual    const nString& getName()             const { return m_name; }
            virtual       const bool& isEnabled()           const { return m_flags.isEnabled; }
            virtual              bool isMouseIn()           const { return m_flags.isMouseIn; }

            virtual volatile bool needsDimensionUpdate()       const { return m_flags.needsDimensionUpdate; }
            virtual volatile bool needsClipRectRecalculation() const { return m_flags.needsClipRectRecalculation; }
            virtual volatile bool needsDrawableReload()        const { return m_flags.needsDrawableRefresh; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setRenderer(UIRenderer* renderer) { removeDrawables(); m_renderer = renderer; addDrawables(); }
            virtual void setGameWindow(GameWindow* window) { m_window = window; }
            /*!
             * \brief Sets the parent widget of this widget.
             * 
             * \warning This function could end up being costly if called too often - has to traverse all descendant widgets and sometimes all ancestor widgets.
             */
            virtual void setParent(IWidget* parent);
            virtual void setFont(const Font* font) { m_font = font; }
            virtual void setPosition(f32v2 position);
            virtual void setX(f32 x);
            virtual void setY(f32 y);
            virtual void setRelativePosition(f32v2 relativePosition);
            virtual void setRelativeX(f32 relX);
            virtual void setRelativeY(f32 relY);
            virtual void setSize(f32v2 size);
            virtual void setWidth(f32 width);
            virtual void setHeight(f32 height);
            virtual void setClipping(Clipping clipping);
            virtual void setClippingLeft(ClippingState state);  
            virtual void setClippingTop(ClippingState state);
            virtual void setClippingRight(ClippingState state);
            virtual void setClippingBottom(ClippingState state);
            virtual void setName(const nString& name) { m_name = name; }

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<const MouseButtonEvent&> MouseClick; ///< Occurs when container is clicked by mouse.
            Event<const MouseButtonEvent&> MouseDown; ///< Occurs when mouse button is pressed over container.
            Event<const MouseButtonEvent&> MouseUp; ///< Occurs when mouse button is released over container.
            Event<const MouseMotionEvent&> MouseEnter; ///< Occurs when mouse pointer enters the container.
            Event<const MouseMotionEvent&> MouseLeave; ///< Occurs when mouse pointer leaves the container.
            Event<const MouseMotionEvent&> MouseMove; ///< Occurs when mouse pointer is moved over container.
            // TODO(Ben): Lots more events!

        protected:
            /*! \brief Updates the dimensions of the new IWidget according to specific widget rules.
             *
             *  The simplest form could be m_position = m_relativePosition;
             */
            virtual void updateDimensions() = 0;

            /*! Removes a widget from a dock and returns true on success. */
            // bool removeChildFromDock(Widget* widget);
            /*! Refreshes all docked widget positions and sizes. */
            // void recalculateDockedWidgets();
            
            /*! Computes clipping for rendering and propagates through children. */
            virtual void calculateClipRect();
            /*! Reset clip rect. */
            virtual void resetClipRect() { m_clipRect = f32v4(-(FLT_MAX), -(FLT_MAX), FLT_MAX, FLT_MAX); };
            /*! Computes the clipping of child widgets. */
            virtual void calculateChildClipRects();
            
            /*!
             * \brief Updates all child widgets' dimensions.
             */
            virtual void updateChildDimensions();
            /*!
             * \brief Updates all child widgets' canvas fields.
             */
            virtual void updateChildCanvases();
            
            /*! @brief Adds all drawables to the UIRenderer. */
            virtual void addDrawables();
            /*! @brief Removes all drawables from the UIRenderer. */
            virtual void removeDrawables();
            /*! @brief Refreshes all drawables. */
            virtual void refreshDrawables() = 0;

            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseDown(Sender s, const MouseButtonEvent& e);
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e);
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e);
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e);
            virtual void onResize(Sender s, const WindowResizeEvent& e);

            /************************************************************************/
            /* LUA Callbacks                                                        */
            /************************************************************************/
#ifdef VORB_USING_SCRIPT
            std::vector<script::Function> m_mouseClickFuncs;
            std::vector<script::Function> m_mouseDownFuncs;
            std::vector<script::Function> m_mouseUpFuncs;
            std::vector<script::Function> m_mouseEnterFuncs;
            std::vector<script::Function> m_mouseLeaveFuncs;
            std::vector<script::Function> m_mouseMoveFuncs;
#endif

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            UIRenderer*       m_renderer;         ///< Renderer to use for drawing widget.
            const GameWindow* m_window;           ///< Game window pointer.
            IWidget*          m_canvas;           /*< Canvas widget - i.e. the oldest ancestor of this widget. If this widget
                                                   *  is the canvas of its children, m_canvas is set to the this pointer (which
                                                   *  is the case by default). NOTE: We should never update this parameter via a
                                                   *  setter, it should only change via private logic on parent changes.
                                                   */
            IWidget*          m_parent;           ///< Parent widget.
            IWidgets          m_widgets;          ///< Collection of child widgets.
            const Font*       m_font;             ///< Font for rendering.
            f32v2             m_position;         ///< Position of widget relative to window in pixels.
            f32v2             m_size;             ///< Size of the widget in pixels.
            Clipping          m_clipping;         ///< Clipping rules to use for generating the clip rectangle.
            f32v4             m_clipRect;         ///< Clipping rectangle for rendering.
            nString           m_name;             ///< Display name of the container.

            WidgetFlags m_flags;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_IContainer_h__