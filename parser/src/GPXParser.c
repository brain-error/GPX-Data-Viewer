/*
CIS 2750 Assignment 2
Name: Sanaz Rezahi
ID: 0957635
Date: October 24, 2019

Sample code from http://www.xmlsoft.org/examples/tree2.c and http://knol2share.blogspot.com/2009/05/validate-xml-against-xsdin-
c.html was used in some functions
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GPXParser.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
#include <HelperFunctions.h>
#include <LinkedListAPI.h>

//*************************ADDED******************************************
bool isValid(char* file) {
  GPXdoc* doc = createGPXdoc(file);
  if (doc == NULL) {
    return false;
  }

  bool success = validateGPXDoc(doc, "gpx.xsd");
  deleteGPXdoc(doc);
  return success;
}
bool renameRT (char* file, int num, char name, char* rename) {
  GPXdoc* doc = createGPXdoc(file);
  if (doc == NULL) {
    return false;
  }

  if (name == 'R') {
    int count = 0;
      for (Node *node = doc->routes->head; node != NULL; node = node->next) {
        count++;
        if (count == num) {
          Route *rte = (Route*)node->data;
          strcpy(rte->name, rename);
        }
      }
  }
  else if (name == 'T') {
    int count = 0;
      for (Node *node = doc->tracks->head; node != NULL; node = node->next) {
        count++;
        if (count == num) {
          Track *trk = (Track*)node->data;
          strcpy(trk->name, rename);
        }
      }
  }
  bool success = writeGPXdoc(doc, file);
  deleteGPXdoc(doc);
  return success;
}

char* attrToJSON (Attribute* attr) {
  char* string = calloc(1000, sizeof(char));
  if (attr == NULL) {
    strcat(string, "{}");
    return string;
  }
  strcat(string, "{\"name\":\"");
  strcat(string, attr->name);
  strcat(string, "\"");
  strcat(string, ",");
  strcat(string, "\"value\":\"");
  strcat(string, attr->value);
  strcat(string, "\"");
  strcat(string, "}");
  return string;
}

char* attrListToJSON (List *list) {
  char* string = calloc(10000, sizeof(char));
  if (list == NULL || list->length == 0) {
    strcat(string, "[]");
    return string;
  }

  strcat(string, "[");
  for (Node *anode = list->head; anode != NULL; anode = anode->next) {
    Attribute *attr = (Attribute*)anode->data;
    char *attribute = attrToJSON(attr);
    strcat(string, attribute);
    free(attribute);
    if (anode->next != NULL) {
      strcat(string, ",");
    }
 }
 strcat(string, "]");

 return string;
}

char* pointToJSON (Waypoint* wpt) {
  char* string = calloc(1000, sizeof(char));
  if (wpt == NULL) {
    strcat(string, "{}");
    return string;
  }
  strcat(string, "{\"name\":\"");
  if (strcmp(wpt->name, "") == 0) {
    strcat(string, "(Empty)");
  }
  else {
    strcat(string, wpt->name);
  }
  strcat(string, "\"");
  strcat(string, ",");
  strcat(string, "\"longitude\": ");
  char lat[100];
  sprintf(lat, "%f", wpt->longitude);
  strcat(string, lat);
  sprintf(lat, "%f", wpt->latitude);
  strcat(string, ",\"latitude\": ");
  strcat(string, lat);
  strcat(string, "}");
  return string;
}

char* pointListToJSON (List *list) {
  char* string = calloc(10000, sizeof(char));
  if (list == NULL || list->length == 0) {
    strcat(string, "[]");
    return string;
  }

  strcat(string, "[");
  for (Node *anode = list->head; anode != NULL; anode = anode->next) {
    Waypoint *wpt = (Waypoint*)anode->data;
    char *waypoint = pointToJSON(wpt);
    strcat(string, waypoint);
    free(waypoint);
    if (anode->next != NULL) {
      strcat(string, ",");
    }
 }
 strcat(string, "]");

  return string;
}

char* getRouteName(char* file, int routeNum) {
  GPXdoc* doc = createGPXdoc(file);

  if (doc == NULL || doc->routes->length == 0) {
    deleteGPXdoc(doc);
    return NULL;
  }

  int count = 0;
  for (Node *node = doc->routes->head; node != NULL; node = node->next) {
    count++;
    if (count == routeNum) {
      Route* rte = (Route*)node->data;
      char* name = calloc(1000, sizeof(char));
      strcpy(name, rte->name);
      deleteGPXdoc(doc);
      return name;
    }
  }
  deleteGPXdoc(doc);
  return NULL;
}

char* routePointToJSON (char* file, int routeNum) {
  GPXdoc* doc = createGPXdoc(file);

  char* string = calloc(10000, sizeof(char));
  if (doc == NULL || doc->routes->length == 0) {
    strcat(string, "[]");
    deleteGPXdoc(doc);
    return string;
  }
  int count = 0;
  for (Node *node = doc->routes->head; node != NULL; node = node->next) {
    count++;
    if (count == routeNum) {
      Route* rte = (Route*)node->data;
      strcpy(string, pointListToJSON(rte->waypoints));
      deleteGPXdoc(doc);
      return string;
    }
  }
  deleteGPXdoc(doc);
  strcat(string, "[]");
  return string;
}

int routeLength(char* file) {
  GPXdoc* doc = createGPXdoc(file);
  if (doc == NULL) {
    return 0;
  }
  int length = 0;
  length = doc->routes->length;
  deleteGPXdoc(doc);
  return length;
}

int pointLength(char* file, int routeNum) {
  GPXdoc* doc = createGPXdoc(file);
  if (doc == NULL) {
    return 0;
  }
  int count = 0;
  int length = 0;
  for (Node *node = doc->routes->head; node != NULL; node = node->next) {
    count++;
    if (count == routeNum) {
      Route* rte = node->data;
      length = rte->waypoints->length;
      deleteGPXdoc(doc);
      return length;
    }
  }
  deleteGPXdoc(doc);
  return length;
}

char* rtaToJSON (char* file, int num, char name) {
  GPXdoc* doc = createGPXdoc(file);

  char* string = calloc(1000, sizeof(char));
  if (doc == NULL) {
    strcat(string, "[]");
    deleteGPXdoc(doc);
    return string;
  }

  //If it is a route
  if (name == 'R') {
    int count = 0;
      for (Node *node = doc->routes->head; node != NULL; node = node->next) {
        count++;
        if (count == num) {
          Route *rte = (Route*)node->data;
          char* routeAttr = attrListToJSON(rte->attributes);
          strcat(string, routeAttr);
          deleteGPXdoc(doc);
          return string;
        }
      }
  }
  //If it is a track
  else if (name == 'T') {
    int count = 0;
      for (Node *node = doc->tracks->head; node != NULL; node = node->next) {
        count++;
        if (count == num) {
          Track *trk = (Track*)node->data;
          char* trackAttr = attrListToJSON(trk->attributes);
          strcat(string, trackAttr);
          deleteGPXdoc(doc);
          return string;
        }
      }
  }
  deleteGPXdoc(doc);
  return string;
}

char* makeFileLog(char* file) {
  GPXdoc* doc = createGPXdoc(file);
  char* string = GPXtoJSON(doc);
  deleteGPXdoc(doc);
  return string;
}

char* routeGPX (char* file) {
  GPXdoc* doc = createGPXdoc(file);
  char* string = gpxRouteListJSON(doc);
  printf("%s\n", string);
  deleteGPXdoc(doc);
  return string;
}

char* trackGPX (char* file) {
  GPXdoc* doc = createGPXdoc(file);
  char* string = gpxTrackListJSON(doc);
  deleteGPXdoc(doc);
  return string;
}

bool newGPX(char* string, char* file) {
  if (string == NULL || strlen(file) == 0) {
    return false;
  }

  GPXdoc* doc = JSONtoGPX(string);
  bool success = writeGPXdoc(doc, file);

  deleteGPXdoc(doc);
  return success;
}

int addRouteToGPX(char* file, char* routeString) {
  printf("File: %s Route: %s", file, routeString);
  int num;
  GPXdoc* doc = createGPXdoc(file);
  Route *rte = JSONtoRoute(routeString);
  addRoute(doc, rte);
  num = doc->routes->length;
  printf("Length: %d", num);
  bool success = writeGPXdoc(doc, file);
  if (success == false) {
    return -1;
  }
  deleteGPXdoc(doc);

  return num;
}

bool addWaypointToGPX(char *file, int num, char* waypointString) {
  printf("File: %s Waypoint: %s", file, waypointString);
  GPXdoc* doc = createGPXdoc(file);
  int count = 0;
  for (Node* node = doc->routes->head; node != NULL; node = node->next) {
    count++;
    if (count == num) {
      Route* rte = (Route*)node->data;
      Waypoint* wpt = JSONtoWaypoint(waypointString);
      addWaypoint(rte, wpt);
    }
  }
  bool success = writeGPXdoc(doc, file);
  deleteGPXdoc(doc);

  return success;
}

char* findRoutes(char* file, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {
    GPXdoc* doc = createGPXdoc(file);
    if (doc == NULL) {
      return NULL;
    }
    List* routes = getRoutesBetween(doc, sourceLat, sourceLong, destLat, destLong, delta);
    char* list = routeListToJSON(routes);
    return list;
}

char* findTracks(char* file, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {
  GPXdoc* doc = createGPXdoc(file);
  if (doc == NULL) {
    return NULL;
  }
  List* tracks = getTracksBetween(doc, sourceLat, sourceLong, destLat, destLong, delta);
  char* list = trackListToJSON(tracks);
  return list;
}
//**************************BONUS******************************************

void addWaypoint(Route *rt, Waypoint *pt) {
  if (rt == NULL || pt == NULL) {
    return;
  }

  insertBack(rt->waypoints, pt);
  return;
}
void addRoute(GPXdoc* doc, Route* rt) {
  if (doc == NULL || rt == NULL) {
    return;
  }

  insertBack(doc->routes, rt);
  return;
}
GPXdoc* JSONtoGPX(const char* gpxString) {
  if (gpxString == NULL) {
    return NULL;
  }

  char string[256];
  char *version;
  char *creator;

  GPXdoc *object = calloc(1, sizeof(GPXdoc));
  if (object == NULL) {
    return NULL;
  }
  object->creator = calloc(1000, sizeof(char));
  if (object->creator == NULL) {
    free(object);
    return NULL;
  }

  //Intialize Lists in gpx_doc
  object->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
  object->routes = initializeList(&routeToString, &deleteRoute, &compareRoutes);
  object->tracks = initializeList(&trackToString, &deleteTrack, &compareTracks);

  //Namespace
  strcpy(object->namespace, "http://www.topografix.com/GPX/1/1");
  strcpy(string, gpxString);
  //Version
  version = strtok(string, ":");
  version = strtok(NULL, ",");

  object->version = atof(version);

  //Creator
  strcpy(string, gpxString);
  creator = strtok(string, "c");
  creator = strtok(NULL, "}");
  creator = strtok(creator, ":");
  creator = strtok(NULL, "}");
  creator = strtok(creator, "\"");
  strcpy(object->creator, creator);

  return object;
}
Waypoint* JSONtoWaypoint(const char* gpxString) {
  if (gpxString == NULL) {
    return NULL;
  }

  char string[256];
  char *lat;
  char *lon;

  Waypoint *wpt = calloc(1, sizeof(Waypoint));
  if (wpt == NULL) {
    return NULL;
  }
  wpt->name = calloc(500, sizeof(char));
  if (wpt->name == NULL) {
    free(wpt);
    return NULL;
  }

  wpt->attributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

  strcpy(string, gpxString);
  //Latitude
  lat = strtok(string, ":");
  lat = strtok(NULL, ",");

  wpt->latitude = atof(lat);
  //Longitude
  strcpy(string, gpxString);
  lon = strtok(string, ",");
  lon = strtok(NULL, "}");
  lon = strtok(lon, ":");
  lon = strtok(NULL, "}");

  wpt->longitude = atof(lon);
  return wpt;
}
Route* JSONtoRoute(const char* gpxString) {
  if (gpxString == NULL) {
    return NULL;
  }

  char string[256];
  char* name;

  Route *rte = calloc(1, sizeof(Route));
  if (rte == NULL) {
    return NULL;
  }
  rte->name = calloc(500, sizeof(char));
  if (rte->name == NULL) {
    free(rte);
    return NULL;
  }

  rte->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
  rte->attributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

  strcpy(string, gpxString);
  //Name
  name = strtok(string, ":");
  name = strtok(NULL, "}");
  name = strtok(name, "\"");
  strcpy(rte->name, name);
  return rte;
}
//**************************Assignment 2 Module 3**************************

char* gpxRouteListJSON(GPXdoc* doc) {
  return routeListToJSON(doc->routes);
}

char* gpxTrackListJSON(GPXdoc* doc) {
  return trackListToJSON(doc->tracks);
}

char* routeListToJSON(const List *list) {
  char* string = calloc(10000, sizeof(char));
  if (list == NULL || list->length == 0) {
    strcat(string, "[]");
    return string;
  }

  strcat(string, "[");
   for (Node *node = list->head; node != NULL; node = node->next) {
     Route *rte = (Route*)node->data;
     char *route = routeToJSON(rte);
     strcat(string, route);
     free(route);
     if (node->next != NULL) {
       strcat(string, ",");
     }
  }
  strcat(string, "]");

  return string;
}

char* trackListToJSON(const List *list){
  char* string = calloc(10000, sizeof(char));
  if (list == NULL || list->length == 0) {
    strcat(string, "[]");
    return string;
  }

  strcat(string, "[");
   for (Node *node = list->head; node != NULL; node = node->next) {
     Track *trk = (Track*)node->data;
     char *track = trackToJSON(trk);
     strcat(string, track);
     free(track);
     if (node->next != NULL) {
       strcat(string, ",");
     }
  }
  strcat(string, "]");

  return string;
}

char* trackToJSON(const Track *tr){
  char* string = calloc(1000, sizeof(char));
  if (tr == NULL) {
    strcat(string, "{}");
    return string;
  }
  strcat(string, "{\"name\":");
  if (strcmp(tr->name, "") == 0) {
    strcat(string, "\"None\"");
  }
  else {
    strcat(string, "\"");
    strcat(string, tr->name);
    strcat(string, "\"");
  }

  char len[50];
  strcat(string, ",\"len\":");
  sprintf(len, "%.1f", round10(getTrackLen(tr)));
  strcat(string, len);
  strcat(string, ",\"loop\":");
  if (isLoopTrack(tr, 15) == true) {
      strcat(string, "true");
  }
  else {
      strcat(string, "false");
  }

  strcat(string, "}");
  return string;
}

char* routeToJSON(const Route *rt){
  char* string = calloc(1000, sizeof(char));
  if (rt == NULL) {
    strcat(string, "{}");
    return string;
  }

  strcat(string, "{\"name\":");
  if (strcmp(rt->name, "") == 0) {
    strcat(string, "\"None\"");
  }
  else {
    strcat(string, "\"");
    strcat(string, rt->name);
    strcat(string, "\"");
  }
  strcat(string, ",\"numPoints\":");
  char len[50];
  sprintf(len, "%d", getLength(rt->waypoints));
  strcat(string, len);
  strcat(string, ",\"len\":");
  sprintf(len, "%.1f", round10(getRouteLen(rt)));
  strcat(string, len);
  strcat(string, ",\"loop\":");
  if (isLoopRoute(rt, 15) == true) {
      strcat(string, "true");
  }
  else {
      strcat(string, "false");
  }

  strcat(string, "}");
  return string;
}

char* GPXtoJSON(const GPXdoc* gpx){
  char* string = calloc(1000, sizeof(char));
  if (gpx == NULL) {
    strcat(string, "{}");
    return string;
  }

  strcat(string, "{\"version\":");
  char num[30];
  sprintf(num, "%.1f", gpx->version);
  strcat(string, num);
  strcat(string, ",\"creator\":");
  strcat(string, "\"");
  strcat(string, gpx->creator);
  strcat(string, "\"");
  strcat(string, ",\"numWaypoints\":");
  sprintf(num, "%d", getLength(gpx->waypoints));
  strcat(string, num);
  strcat(string, ",\"numRoutes\":");
  sprintf(num, "%d", getLength(gpx->routes));
  strcat(string, num);
  strcat(string, ",\"numTracks\":");
  sprintf(num, "%d", getLength(gpx->tracks));
  strcat(string, num);
  strcat(string, "}");
  return string;
}

//**************************Assignment 2 Module 2**************************
float getRouteLen(const Route *rt) {
  if (rt == NULL) {
    return 0;
  }

  float dist = 0; //Route length

  //Loop through Route waypoint List
  for (Node *node = rt->waypoints->head; node != NULL; node = node->next) {
    if (node->next != NULL) {
      Waypoint *wpt1 = (Waypoint*)node->data;
      Waypoint *wpt2 = (Waypoint*)node->next->data;

      dist += haversine(wpt1, wpt2);
    }
  }
  return dist;
}

float getTrackLen(const Track *tr) {
  float dist = 0;

  if (tr == NULL) {
    return 0;
  }

  if (tr->segments->head->next == NULL) {
  //Loop through TrackSeg Waypoint List
    TrackSegment *seg = (TrackSegment*)tr->segments->head->data;
    for (Node *node = seg->waypoints->head; node != NULL; node = node->next) {
      if (node->next != NULL) {
        Waypoint *wpt1 = (Waypoint*)node->data;
        Waypoint *wpt2 = (Waypoint*)node->next->data;

        dist += haversine(wpt1, wpt2);
      }
    }
    return dist;
  }
  else {
    Waypoint *last = NULL;
    for (Node *nodeS = tr->segments->head; nodeS != NULL; nodeS = nodeS->next) {
      TrackSegment *seg = (TrackSegment*)nodeS->data;
      //Loop through TrackSeg Waypoint List
      for (Node *nodeW = seg->waypoints->head; nodeW != NULL; nodeW = nodeW->next) {
        if (nodeW->next != NULL) {
          Waypoint *wpt1 = (Waypoint*)nodeW->data;
          Waypoint *wpt2 = (Waypoint*)nodeW->next->data;

          dist += haversine(wpt1, wpt2);
        }
        else {
          last = (Waypoint*)nodeW->data; //Stores last waypoint
        }
      }
      //Calculates distance between last waypoint of current seg and first waypoint of next seg
      if (nodeS->next) {
        TrackSegment *seg = (TrackSegment*)nodeS->next->data;
        Waypoint *first = (Waypoint*)seg->waypoints->head->data;

        dist += haversine(last, first);
      }
    }
    return dist;
  }

  return dist;
}

float round10(float len) {
  if (len == 0) {
    return len;
  }
  float x = fmod(len, 10);
  if (x > 0) {
    if (x >= 5) {
      len = 10-x + len;
    }
    else {
      len = len - x;
    }
  }
  return len;
}

int numRoutesWithLength(const GPXdoc* doc, float len, float delta) {
  if (doc == NULL) {
    return 0;
  }
  if (len < 0 || delta < 0) {
    return 0;
  }
  int count = 0;
  for (Node *node = doc->routes->head; node != NULL; node = node->next) {
    Route *rte = (Route*)node->data;
    if (abs(getRouteLen(rte)-len) <= delta) {
      count++;
    }
  }
  return count;
}

int numTracksWithLength(const GPXdoc* doc, float len, float delta) {
  if (doc == NULL) {
    return 0;
  }
  if (len < 0 || delta < 0) {
    return 0;
  }
  int count = 0;
  for (Node *node = doc->tracks->head; node != NULL; node = node->next) {
    Track *trk = (Track*)node->data;
    if (abs(getTrackLen(trk)-len) <= delta) {
      count++;
    }
  }
  return count;
}

bool isLoopRoute(const Route* route, float delta) {
  if (route == NULL || delta < 0) {
    return false;
  }
  if (route->waypoints->length == 0){
    return false;
  }
  Waypoint *last = NULL;
  Waypoint *first = (Waypoint*)route->waypoints->head->data;  //Get first waypoint from route wpt list
  for (Node *node = route->waypoints->head; node != NULL; node = node->next ) {
    if (node->next == NULL) {
       last = (Waypoint*)node->data;
    }
  }
  float dist = haversine(first, last);
  if (getLength(route->waypoints) >= 4 && dist <= delta) {
    return true;
  }

  return false;
}

bool isLoopTrack(const Track *tr, float delta) {
  if (tr == NULL || delta < 0) {
    return false;
  }
  Waypoint *last = NULL;
  int size = 0;

  TrackSegment *trkseg = (TrackSegment*)tr->segments->head->data; //Get first waypoint from trackseg wpt list
  Waypoint *first = (Waypoint*)trkseg->waypoints->head->data;
  for (Node *node = tr->segments->head; node != NULL; node = node->next ) {
    TrackSegment *seg = (TrackSegment*)node->data;
    size += getLength(seg->waypoints);
    if (node->next == NULL) {
      for (Node *n = seg->waypoints->head; n != NULL; n = n->next ) {
        if (n->next == NULL) {
          last = (Waypoint*)n->data;
        }
      }
    }
  }
  float dist = haversine(first, last);
  if (size >= 4 && dist <= delta) {
    return true;
  }

  return false;
}

List* getRoutesBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {
  if (doc == NULL) {
    return NULL;
  }
  if (delta < 0) {
    return NULL;
  }
  //Initialize Root List
  List *routeList = initializeList(&routeToString, &deleteRoute, &compareRoutes);

  //Make source waypoints
  Waypoint *firstI = intializeWaypoint(sourceLat, sourceLong);
  Waypoint *lastI = intializeWaypoint(destLat, destLong);

  //Cycle through routes list in doc to get first and last waypoints of route
  for (Node* node = doc->routes->head; node != NULL; node = node->next) {
    Route *route = (Route*)node->data;
    Waypoint *lastR = NULL;
    Waypoint *firstR = (Waypoint*)route->waypoints->head->data;  //Get first waypoint from route wpt list
    for (Node *node2 = route->waypoints->head; node2 != NULL; node2 = node2->next ) {
      if (node2->next == NULL) {
         lastR = (Waypoint*)node2->data;
      }
    }
    float distFirst = haversine(firstI, firstR);
    float distLast = haversine(lastI, lastR);
    //Compare to latitudes and longitutdes
    if (distFirst <= delta && distLast <= delta) {
      Route *rte = copyRoute(route);
      insertBack(routeList, rte);
    }
  }

  deleteWaypoint(firstI);
  deleteWaypoint(lastI);
  if (getLength(routeList) == 0) {
    freeList(routeList);
    return NULL;
  }
  return routeList;
}

List* getTracksBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {
  if (doc == NULL) {
    return NULL;
  }
  if (delta < 0) {
    return NULL;
  }
  //Initialize Root List
  List *trackList = initializeList(&trackToString, &deleteTrack, &compareTracks);

  //Make source waypoints
  Waypoint *firstI = intializeWaypoint(sourceLat, sourceLong);
  Waypoint *lastI = intializeWaypoint(destLat, destLong);

  //Cycle through tracks list in doc to get distance

  for (Node* node = doc->tracks->head; node != NULL; node = node->next) {
    Track *track = (Track*)node->data;
    TrackSegment *trkseg = (TrackSegment*)track->segments->head->data; //Get first waypoint from trackseg wpt list
    Waypoint *lastT = NULL;
    Waypoint *firstT = (Waypoint*)trkseg->waypoints->head->data;
    for (Node *node2 = track->segments->head; node2 != NULL; node2 = node2->next ) {
      TrackSegment *seg = (TrackSegment*)node2->data;
      if (node2->next == NULL) {
        for (Node *n = seg->waypoints->head; n != NULL; n = n->next ) {
          if (n->next == NULL) {
            lastT = (Waypoint*)n->data;
          }
        }
      }
    }
    float distFirst = haversine(firstI, firstT);
    float distLast = haversine(lastI, lastT);
    //Compare to latitudes and longitutdes
    if (distFirst <= delta && distLast <= delta) {
      Track *trk = copyTrack(track);
      insertBack(trackList, trk);
    }
  }

  deleteWaypoint(firstI);
  deleteWaypoint(lastI);
  if ((getLength(trackList) == 0)) {
    freeList(trackList);
    return NULL;
  }
  return trackList;
}

//**************************Assignment 2 Module 1***************************
bool validateGPXDoc(GPXdoc* doc, char* gpxSchemaFile) {
  //Check parameters
  if ((doc == NULL) || (gpxSchemaFile == NULL)) {
    return false;
  }

  //Check if it is a .xsd
  const char *ext = strrchr (gpxSchemaFile, '.');
  if (ext == NULL) {
    return false;
  }
  if (strcmp (ext, ".xsd") != 0) {
    return false;
  }

  //Check if file is empty
  FILE *f;
  f = fopen(gpxSchemaFile, "r");

  if (f == NULL) {
    fclose(f);
    return false;
  }
  else {
    fseek(f, 0, SEEK_END);
    int i = ftell(f);

    if (i == 0) {
      return false;
    }

    fclose(f);
  }



  xmlSchemaPtr schema = NULL;
  xmlSchemaParserCtxtPtr ctxt;

  xmlLineNumbersDefault(1);

  ctxt = xmlSchemaNewParserCtxt(gpxSchemaFile);

  xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
  schema = xmlSchemaParse(ctxt);
  xmlSchemaFreeParserCtxt(ctxt);

  //Check if xml was converted into libxml tree
  xmlDocPtr xml = NULL;
  xml = convertGPX(doc);
  if (xml == NULL) {
    return false;
  }
  else {
    xmlSchemaValidCtxtPtr ctxt;
    int ret;

    ctxt = xmlSchemaNewValidCtxt(schema);
    xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    ret = xmlSchemaValidateDoc(ctxt, xml);
    if (ret == 0) {
      xmlSchemaFreeValidCtxt(ctxt);
      xmlFreeDoc(xml);

      if(schema != NULL)
      xmlSchemaFree(schema);

      xmlSchemaCleanupTypes();
      xmlMemoryDump();

      return true;
    }
    else if (ret > 0) {
      xmlSchemaFreeValidCtxt(ctxt);
      xmlFreeDoc(xml);

      if(schema != NULL)
      xmlSchemaFree(schema);

      xmlSchemaCleanupTypes();
      xmlMemoryDump();

      return false;
    }
    else {
      xmlSchemaFreeValidCtxt(ctxt);
      xmlFreeDoc(xml);

      if(schema != NULL)
      xmlSchemaFree(schema);

      xmlSchemaCleanupTypes();
      xmlMemoryDump();

      return false;
    }
  }

  if(schema != NULL)
  xmlSchemaFree(schema);

  xmlSchemaCleanupTypes();
  xmlMemoryDump();

  return false;
}

bool writeGPXdoc(GPXdoc* doc, char* fileName) {
  if ((doc == NULL) || (fileName == NULL)) {
    return false;
  }

  const char *ext = strrchr (fileName, '.');
  if (ext == NULL) {
    return false;
  }
  if (strcmp (ext, ".gpx") != 0) {
    return false;
  }

  xmlDocPtr xml = NULL;

  xml = convertGPX(doc);
  if (xml == NULL) {
    return false;
  }

  xmlSaveFormatFileEnc(fileName, xml, "UTF-8", 1);

  bool result = validateGPXDoc(doc, "gpx.xsd");

  if(result == false) {
    return false;
  }

  xmlFreeDoc(xml);
  xmlMemoryDump();
  return true;
}

//**************************Assignment 1 Helper functions***************************
GPXdoc* createGPXdoc(char* fileName) {
  LIBXML_TEST_VERSION

  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;

  //Set doc using filename
  doc = xmlReadFile(fileName, NULL, 0);
  //Check if doc is NULL
  if (doc == NULL) {
      return NULL;
  }

  //Get the root element node
  root_element = xmlDocGetRootElement(doc);
  if (root_element == NULL) {
      return NULL;
  }
  //Intialize GPXdoc object
  GPXdoc *object = calloc(1, sizeof(GPXdoc));
  if (object == NULL) {
    return NULL;
  }
  object->creator = calloc(1000, sizeof(char));
  if (object->creator == NULL) {
    free(object);
    return NULL;
  }

  //Intialize Lists in gpx_doc
  object->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
  object->routes = initializeList(&routeToString, &deleteRoute, &compareRoutes);
  object->tracks = initializeList(&trackToString, &deleteTrack, &compareTracks);

  //Parse and create GPXdoc object, then update
  object = createGPXObject(root_element, object, doc);
  if (object == NULL) {
    deleteGPXdoc(object);
    return NULL;
  }

  //Free the XML doc file
  xmlFreeDoc(doc);

  //Return GPXobject pointer
  return object;
}

char* GPXdocToString(GPXdoc* doc) {
  if (doc == NULL) {
    return NULL;
  }
  char* string = calloc(100000, sizeof(char));
  strcat(string, "\nPrinting GPXdoc....\nNamespace: ");
  strcat(string, doc->namespace);
  strcat(string, "\nCreator: ");
  strcat(string, doc->creator);
  strcat(string, "\nVersion: ");
  char vr[10];
  sprintf(vr, "%f", doc->version);
  strcat(string, vr);
  char* wpt = toString(doc->waypoints);
  strcat(string, wpt);
  free(wpt);
  char* rte = toString(doc->routes);
  strcat(string, rte);
  free(rte);
  char* trk = toString(doc->tracks);
  strcat(string, trk);
  free(trk);
  return string;
}

void deleteGPXdoc(GPXdoc* doc) {

  if (doc != NULL) {
    freeList(doc->routes);
    freeList(doc->waypoints);
    freeList(doc->tracks);
    free(doc->creator);
    free(doc);
  }


}

//**************************Assignment 1 Helper functions***************************
//Attributes
void deleteAttribute(void* data) {
  if (data == NULL) {
    return;
  }

  Attribute* temp = (Attribute*)data;

  free(temp->name);
  free(temp->value);
  free(temp);
}

char* attributeToString(void* data) {
  if (data == NULL) {
    return NULL;
  }

  Attribute* temp = (Attribute*)data;

  char* string = calloc(10000, sizeof(char));

  strcat(string, "\nAttributes\n");
  strcat(string, "Name: ");
  strcat(string, temp->name);
  strcat(string, "\nValue: ");
  strcat(string, temp->value);
  strcat(string, "\n");

  return string;
}

int compareAttributes(const void* first, const void* second) {
  return 0;
}

//Waypoints
void deleteWaypoint(void* data) {
  if (data == NULL) {
    return;
  }

  Waypoint* temp = (Waypoint*) data;

  free(temp->name);
  freeList(temp->attributes);
  free(temp);
}

char* waypointToString(void* data) {
    if (data == NULL) {
      return NULL;
    }

    Waypoint* temp = (Waypoint*)data;

    char* string = calloc(10000, sizeof(char));

    strcpy(string, "\nWaypoint\n");
    strcat(string, "Name: ");
    strcat(string, (char*)temp->name);
    strcat(string, "\nLatitude: ");
    char lat[100];
    sprintf(lat, "%f", temp->latitude);
    strcat(string, lat);
    strcat(string, "\nLongitude: ");
    sprintf(lat, "%f", temp->longitude);
    strcat(string, lat);
    strcat(string, "\n");

   if (temp->attributes->head != NULL) {
      char *attr = toString(temp->attributes);
      strcat(string, attr);
      free(attr);
    }

    return string;

}

int compareWaypoints(const void* first, const void* second) {
  return 0;
}

//Routes
void deleteRoute(void* data) {
  if (data == NULL) {
    return;
  }

  Route* temp = (Route*) data;

  free(temp->name);
  freeList(temp->waypoints);
  freeList(temp->attributes);
  free(temp);
}

char* routeToString(void* data) {
  if (data == NULL) {
    return NULL;
  }

  Route* temp = (Route*)data;

  char* string = calloc(10000, sizeof(char));

  strcat(string, "\nRoute\n");
  strcat(string, "Name: ");
  strcat(string, temp->name);
  strcat(string, "\n");

  if (temp->attributes->head != NULL) {
     char *attr = toString(temp->attributes);
     strcat(string, attr);
     free(attr);
   }

  if (temp->waypoints->head != NULL) {
    char *wpt = toString(temp->waypoints);
    strcat(string, wpt);
    free(wpt);
  }

   return string;
}

int compareRoutes(const void* first, const void* second) {
  /* Make first and second two separate Routes
    Make two char* strings
    Call toString on first and second and store them into respective strings
    Strcmp the strings
    free strings
    */
    return 0;
}

//TrackSegments
void deleteTrackSegment(void* data) {
  if (data == NULL) {
    return;
  }

  TrackSegment *temp = (TrackSegment*)data;

  freeList(temp->waypoints);
  free(temp);
}

char* trackSegmentToString(void* data) {
  if (data == NULL) {
    return NULL;
  }

  TrackSegment* temp = (TrackSegment*)data;

  char* string = calloc(10000, sizeof(char));

  strcat(string, "\nTrackSegment\n");

  if (temp->waypoints->head != NULL) {
    char *wpt = toString(temp->waypoints);
    strcat(string, wpt);
    free(wpt);
  }

   return string;
}

int compareTrackSegments(const void* first, const void* second) {
  return 0;
}

//Tracks

void deleteTrack(void* data) {
  if (data == NULL) {
    return;
  }

  Track *temp = (Track*)data;

  free(temp->name);
  freeList(temp->attributes);
  freeList(temp->segments);
  free(temp);
}

char* trackToString(void* data) {
  if (data == NULL) {
    return NULL;
  }

  Track *temp = (Track*)data;
  char* string = calloc(10000, sizeof(char));

  strcat(string, "\nTrack\n");
  strcat(string, "Name: ");
  strcat(string, temp->name);
  strcat(string, "\n");

  if (temp->attributes->head != NULL) {
    char *attr = toString(temp->attributes);
     strcat(string, attr);
     free(attr);
   }

  if (temp->segments->head != NULL) {
    char *seg = toString(temp->segments);
    strcat(string, seg);
    free(seg);
  }

  return string;
}

int compareTracks(const void* first, const void* second) {
  return 0;
}

//*****************************Assignment 1 Module 2: get functions*************************************
int getNumWaypoints(const GPXdoc* doc) {
  if (doc == NULL) {
    return 0;
  }
  int numWpt = 0;
  numWpt = countFunction(doc, 1);
  return numWpt;
}

int getNumRoutes(const GPXdoc* doc) {
  if (doc == NULL) {
    return 0;
  }
  int numRte = 0;
  numRte = countFunction(doc, 2);
  return numRte;
}

int getNumTracks(const GPXdoc* doc) {
  if (doc == NULL) {
    return 0;
  }
  int numTrk = 0;
  numTrk = countFunction(doc, 3);
  return numTrk;
}

int getNumSegments(const GPXdoc* doc) {
  if (doc == NULL) {
    return 0;
  }
  int numSeg = 0;
  numSeg = countFunction(doc, 4);
  return numSeg;
}

int getNumAttributes(const GPXdoc* doc) {
  if (doc == NULL) {
    return 0;
  }
  int numAttr = 0;
  numAttr = countFunction(doc, 5);
  return numAttr;
}

Waypoint* getWaypoint(const GPXdoc* doc, char* name) {
  if (doc == NULL || name == NULL)   return NULL;

  Node *listNode = NULL;
  for (listNode = doc->waypoints->head; listNode != NULL; listNode = listNode->next) {
    Waypoint *wpt = (Waypoint*)listNode->data;
    if (strcmp(name, wpt->name) == 0) {
      return wpt;
    }
  }

  return NULL;
}

Route* getRoute(const GPXdoc* doc, char* name) {
  if (doc == NULL || name == NULL)   return NULL;

  Node *listNode = NULL;
  for (listNode = doc->routes->head; listNode != NULL; listNode = listNode->next) {
    Route *rte = (Route*)listNode->data;
    if (strcmp(name, rte->name) == 0) {
      return rte;
    }
  }

  return NULL;
}

Track* getTrack(const GPXdoc* doc, char* name) {
  if (doc == NULL || name == NULL)   return NULL;

  Node *listNode = NULL;
  for (listNode = doc->tracks->head; listNode != NULL; listNode = listNode->next) {
    Track *trk = (Track*)listNode->data;
    if (strcmp(name, trk->name) == 0) {
      return trk;
    }
  }

  return NULL;
}
