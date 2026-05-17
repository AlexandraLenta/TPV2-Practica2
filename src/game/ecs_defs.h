// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// Components list - must have at least one element
//
struct Transform;
struct Image;
struct FramedImage;
struct FoodInfo;
struct GhostInfo;
struct Health;
struct Immunity;

#define _CMPS_LIST_ \
	Transform, \
	Image, \
	FramedImage, \
	FoodInfo, \
	GhostInfo, \
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
class RenderSystem;
class PacManSystem;
class CollisionsSystem;
class GhostSystem;
class ImmunitySystem;
class FoodSystem;

#define _SYS_LIST_ \
	RenderSystem, \
	PacManSystem, \
	CollisionsSystem, \
	GhostSystem, \
	FoodSystem, \
	ImmunitySystem