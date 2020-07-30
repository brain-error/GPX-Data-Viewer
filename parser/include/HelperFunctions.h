/*
CIS 2750 Assignment 1
Name: Sanaz Rezahi
ID: 0957635
Date: October 3, 2019

Sample code from http://www.xmlsoft.org/examples/tree1.c was used for reference in some functions.
*/

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <stdio.h>

GPXdoc* createGPXObject(xmlNode * a_node, GPXdoc* doc, xmlDoc* xml);
Waypoint* createWaypoint(xmlNode *node, GPXdoc* doc, xmlDoc* xml);
Attribute* createAttribute(GPXdoc* doc, xmlDoc* xml);
Route* createRoute(xmlNode *node, GPXdoc* doc, xmlDoc* xml);
Track* createTrack(xmlNode *node, GPXdoc* doc, xmlDoc* xml);
TrackSegment* createTrackSegment(xmlNode *node, GPXdoc* doc, xmlDoc* xml);
int countFunction (const GPXdoc *doc, int num);
xmlDocPtr convertGPX(GPXdoc *doc);
float haversine (Waypoint* wpt1, Waypoint* wpt2);
Waypoint* intializeWaypoint(float lat, float lon);
Route* copyRoute(Route *route);
Attribute* intializeAttr();
Track* copyTrack(Track *track);


#endif
