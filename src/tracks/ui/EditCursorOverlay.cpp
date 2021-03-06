/**********************************************************************

Audacity: A Digital Audio Editor

EditCursorOverlay.cpp

Paul Licameli split from TrackPanel.cpp

**********************************************************************/

#include "../../Audacity.h"
#include "EditCursorOverlay.h"
#include "../../Experimental.h"

#include "../../AColor.h"
#include "../../widgets/Ruler.h"
#include "../../Project.h"
//#include "../../TrackPanel.h"
#include "../../TrackPanelCell.h"
#include "../../TrackPanelCellIterator.h"
#include "../../TrackPanelAx.h"
#include "../../ViewInfo.h"

#include <wx/dc.h>

namespace {
   template < class LOW, class MID, class HIGH >
   bool between_incexc(LOW l, MID m, HIGH h)
   {
      return (m >= l && m < h);
   }
}

EditCursorOverlay::EditCursorOverlay(AudacityProject *project)
   : mProject(project)
   , mLastCursorX(-1)
   , mCursorTime(-1)
   , mNewCursorX(-1)
{
}

EditCursorOverlay::~EditCursorOverlay()
{
}

std::pair<wxRect, bool> EditCursorOverlay::DoGetRectangle(wxSize size)
{
   const SelectedRegion &selection = mProject->GetSelection();
   if (!selection.isPoint()) {
      mCursorTime = -1.0;
      mNewCursorX = -1;
   }
   else {
      mCursorTime = selection.t0();
      mNewCursorX = mProject->GetZoomInfo().TimeToPosition
         (mCursorTime, mProject->GetTrackPanel()->GetLeftOffset());
   }

   return std::make_pair(
      mLastCursorX == -1
         ? wxRect()
         : wxRect(mLastCursorX, 0, 1, size.GetHeight()),
      mLastCursorX != mNewCursorX
   );
}


void EditCursorOverlay::Draw
   (wxDC &dc, TrackPanelCellIterator begin, TrackPanelCellIterator end)
{
   mLastCursorX = mNewCursorX;
   if (mLastCursorX == -1)
      return;

   const ZoomInfo &viewInfo = mProject->GetZoomInfo();

   const bool
      onScreen = between_incexc(viewInfo.h,
         mCursorTime,
         mProject->GetScreenEndTime());

   if (!onScreen)
      return;

   AColor::CursorColor(&dc);

   // Draw cursor in all selected tracks
   for (; begin != end; ++begin)
   {
      TrackPanelCellIterator::value_type data(*begin);
      Track *const pTrack = data.first;
      if (!pTrack)
         continue;
      if (pTrack->GetSelected() ||
          mProject->GetTrackPanel()->GetAx().IsFocused(pTrack))
      {
         const wxRect &rect = data.second;
         // AColor::Line includes both endpoints so use GetBottom()
         AColor::Line(dc, mLastCursorX, rect.GetTop(), mLastCursorX, rect.GetBottom()); // <-- The whole point of this routine.

#ifdef EXPERIMENTAL_OUTPUT_DISPLAY
         if (MONO_WAVE_PAN(t)){
            y = t->GetY(true) - mViewInfo->vpos + 1;
            top = y + kTopInset;
            bottom = y + t->GetHeight(true) - kTopInset;
            AColor::Line(dc, mLastCursorX, top, mLastCursorX, bottom);
         }
#endif

      }
   }

   // AS: Ah, no, this is where we draw the blinky thing in the ruler.
   mProject->GetRulerPanel()->Refresh();

   // This updates related displays such as numbers on the status bar
   mProject->TP_DisplaySelection();
}
