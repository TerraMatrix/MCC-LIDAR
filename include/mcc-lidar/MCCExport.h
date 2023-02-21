/* -----------------------------------------------------------------------------
 *  Copyright (c) 2022.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once
#ifndef MCC_MCCEXPORT_H
#define MCC_MCCEXPORT_H

#ifdef MCC_LIDAR_API
#ifdef MCC_LIDAR_EXPORTS
#undef MCC_LIDAR_EXPORTS
#endif
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define MCC_LIDAR_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define MCC_LIDAR_EXPORTS __attribute__((visibility("default")))
#endif
#else
#ifdef MCC_LIDAR_EXPORTS
#undef MCC_LIDAR_EXPORTS
#endif
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define MCC_LIDAR_EXPORTS __declspec(dllimport)
#else
#ifndef MCC_LIDAR_EXPORTS
#define MCC_LIDAR_EXPORTS
#endif
#endif
#endif

#ifdef MCC_LIDAR_API
#ifdef MCC_LIDAR_FUNCTION_EXPORTS
#undef MCC_LIDAR_FUNCTION_EXPORTS
#endif
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define MCC_LIDAR_FUNCTION_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define MCC_LIDAR_FUNCTION_EXPORTS __attribute__((visibility("default")))
#endif
#else
#ifdef MCC_LIDAR_FUNCTION_EXPORTS
#undef MCC_LIDAR_FUNCTION_EXPORTS
#endif
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define MCC_LIDAR_FUNCTION_EXPORTS
#else
#ifndef MCC_LIDAR_FUNCTION_EXPORTS
#define MCC_LIDAR_FUNCTION_EXPORTS
#endif
#endif
#endif

#endif
