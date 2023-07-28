#pragma once
/*
Author(s): Kody Wood
Description: Macros that can be included project wide to help draw debug primatives
*/


#include "DrawDebugHelpers.h"

//MACROS
#define DRAW_DEBUG_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.0f, 12, FColor::Purple, true, 30.0f);
#define DRAW_DEBUG_SPHERE_COLOR(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.0f, 12, Color, true, 30.0f);
#define DRAW_DEBUG_SPHERE_TIME(Location, Duration) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.0f, 12, Color, true, Duration);
#define DRAW_DEBUG_SPHERE_SINGLEFRAME(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.0f, 12, FColor::Purple, false, -1.0f);
#define DRAW_DEBUG_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.0f, 0, 1.0f);
#define DRAW_DEBUG_LINE_SINGLEFRAME(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f, 0, 1.0f);
#define DRAW_DEBUG_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.0f, FColor::Green, true, -1.0f, 0);
#define DRAW_DEBUG_POINT_SINGLEFRAME(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.0f, FColor::Green, false, -1.0f, 0);
#define DRAW_DEBUG_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.0f, 0, 1.0f);\
	DrawDebugPoint(GetWorld(), EndLocation, 15.0f, FColor::Red, true, -1.0f, 0);\
}
#define DRAW_DEBUG_VECTOR_SINGLEFRAME(StartLocation, EndLocation) if(GetWorld()) \
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f, 0, 1.0f);\
	DrawDebugPoint(GetWorld(), EndLocation, 15.0f, FColor::Red, false, -1.0f, 0);\
}
