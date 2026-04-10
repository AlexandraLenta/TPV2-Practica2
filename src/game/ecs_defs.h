// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// Components list - must have at least one element
//
struct Transform;
struct Image;
struct FramedImage;
struct Points;
struct FoodInfo;
struct Health;
struct Immunity;

#define _CMPS_LIST_ \
	Transform, \
	Image, \
	Points, \
	FramedImage, \
	FoodInfo, \
	Health, \
	Immunity

// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	GHOSTS, \
	FRUIT

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
    PACMAN

// Systems list - must have at least one element
//
class StarsSystem;
class RenderSystem;
class PacManSystem;
class GameCtrlSystem;
class CollisionsSystem;
class GhostSystem;
class ImmunitySystem;
class FoodSystem;

#define _SYS_LIST_ \
    StarsSystem, \
	RenderSystem, \
	PacManSystem, \
	GameCtrlSystem, \
	CollisionsSystem, \
	GhostSystem, \
	FoodSystem, \
	ImmunitySystem