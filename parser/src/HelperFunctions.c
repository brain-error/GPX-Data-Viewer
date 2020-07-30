/*
CIS 2750 Assignment 2
Name: Sanaz Rezahi
ID: 0957635
Date: October 24, 2019

Sample code from http://www.xmlsoft.org/examples/tree1.c was used for reference in some functions.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GPXParser.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
#include <HelperFunctions.h>

//******************Assignment 1*********************

GPXdoc* createGPXObject(xmlNode * a_node, GPXdoc* doc, xmlDoc* xml) {
  if (doc == NULL) {
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }
  xmlNode *cur_node = NULL;

  for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
    char* ns = (char *)cur_node->name;
      if (strcmp(ns,"gpx") == 0) {
        //Hard code in the namespace
        strcpy(doc->namespace, "http://www.topografix.com/GPX/1/1");
        //Loop through attributes
        xmlAttr *attr;
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)value->content;

            if (strcmp(attrName, "creator") == 0) {
              strcpy(doc->creator, cont);
            }
            else if (strcmp(attrName, "version") == 0) {
              doc->version = atof(cont);
            }
        }
      }
      else if (strcmp(ns, "wpt") == 0) {
        insertBack(doc->waypoints, createWaypoint(cur_node, doc, xml));
      }
      else if (strcmp(ns, "rte") == 0) {
        insertBack(doc->routes, createRoute(cur_node, doc, xml));
      }
      else if (strcmp(ns, "trk") == 0) {
        insertBack(doc->tracks, createTrack(cur_node, doc, xml));
      }
    createGPXObject(cur_node->children, doc, xml);
  }
  return doc;
}

Track* createTrack(xmlNode *node, GPXdoc* doc, xmlDoc* xml) {
  xmlNode *curNode = NULL;

  Track *trk = calloc(1, sizeof(Track));
  if (trk == NULL) {
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }
  trk->name = calloc(500, sizeof(char));
  if (trk->name == NULL) {
    free(trk);
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }

  trk->segments = initializeList(&trackSegmentToString, &deleteTrackSegment, &compareTrackSegments);
  trk->attributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

  for (curNode = node->children->next; curNode != NULL; curNode = curNode->next->next) {
      if (strcmp((char*)curNode->name, "name") == 0) {
        strcpy((char*)trk->name, (char*)curNode->children->content);
      }
      else if (strcmp((char*)curNode->name, "trkseg") == 0) {
        insertBack(trk->segments, createTrackSegment(curNode, doc, xml));
      }
      else if(strcmp((char*)curNode->name, "name") != 0 && curNode->name != NULL) {
        Attribute *temp = createAttribute(doc, xml);
        strcpy((char*)temp->name, (char*)curNode->name);
        strcpy((char*)temp->value, (char*)curNode->children->content);
        insertBack(trk->attributes, temp);
      }
  }

  return trk;
}

TrackSegment* createTrackSegment(xmlNode *node, GPXdoc* doc, xmlDoc* xml) {
  xmlNode *curNode = NULL;

  TrackSegment *trkseg = calloc(1, sizeof(TrackSegment));
  if (trkseg == NULL) {
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }

  trkseg->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);

  for (curNode = node->children->next; curNode != NULL; curNode = curNode->next->next) {
      if (strcmp((char*)curNode->name, "trkpt") == 0) {
        insertBack(trkseg->waypoints, createWaypoint(curNode, doc, xml));
      }
  }

  return trkseg;
}

Route* createRoute(xmlNode *node, GPXdoc* doc, xmlDoc* xml) {
  xmlNode *curNode = NULL;

  Route *rte = calloc(1, sizeof(Route));
  if (rte == NULL) {
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }
  rte->name = calloc(500, sizeof(char));
  if (rte->name == NULL) {
    free(rte);
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }

  rte->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
  rte->attributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

  for (curNode = node->children->next; curNode != NULL; curNode = curNode->next->next) {
      if (strcmp((char*)curNode->name, "name") == 0) {
        strcpy((char*)rte->name, (char*)curNode->children->content);
      }
      else if (strcmp((char*)curNode->name, "rtept") == 0) {
        insertBack(rte->waypoints, createWaypoint(curNode, doc, xml));
      }
      else if(strcmp((char*)curNode->name, "name") != 0 && curNode->name != NULL) {
        Attribute *temp = createAttribute(doc, xml);
        strcpy((char*)temp->name, (char*)curNode->name);
        strcpy((char*)temp->value, (char*)curNode->children->content);
        insertBack(rte->attributes, temp);
      }
  }

  return rte;
}

Waypoint* createWaypoint(xmlNode *node, GPXdoc* doc, xmlDoc* xml) {
    xmlNode *curNode = NULL;

    Waypoint *wpt = calloc(1, sizeof(Waypoint));
    if (wpt == NULL) {
      deleteGPXdoc(doc);
      xmlFreeDoc(xml);
      return NULL;
    }
    wpt->name = calloc(500, sizeof(char));
    if (wpt->name == NULL) {
      free(wpt);
      deleteGPXdoc(doc);
      xmlFreeDoc(xml);
      return NULL;
    }

    wpt->attributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    xmlAttr *attr;
    for (attr = node->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char*)value->content;

        if (strcmp(attrName, "lat") == 0) {
          wpt->latitude = atof(cont);
        }
        else if (strcmp(attrName, "lon") == 0) {
          wpt->longitude = atof(cont);
        }
    }

    if (strcmp((char*)node->name, "wpt") == 0) {
      for (curNode = node->children->next; curNode != NULL; curNode = curNode->next->next) {
          if (strcmp((char*)curNode->name, "name") == 0) {
            strcpy((char*)wpt->name, (char*)curNode->children->content);
          }
          else if(strcmp((char*)curNode->name, "name") != 0 && curNode->name != NULL) {
              Attribute *temp = createAttribute(doc, xml);
              strcpy((char*)temp->name, (char*)curNode->name);
              strcpy((char*)temp->value, (char*)curNode->children->content);
              insertBack(wpt->attributes, temp);
          }
      }
    }
    else if ((strcmp((char*)node->name, "rtept") == 0) || (strcmp((char*)node->name, "trkpt")) == 0) {
        if (node->children != NULL) {
          for (curNode = node->children->next; curNode != NULL; curNode = curNode->next->next) {
              if (strcmp((char*)curNode->name, "name") == 0) {
                strcpy((char*)wpt->name, (char*)curNode->children->content);
              }
              else if(strcmp((char*)curNode->name, "name") != 0 && curNode->name != NULL) {
                  Attribute *temp = createAttribute(doc, xml);
                  strcpy((char*)temp->name, (char*)curNode->name);
                  strcpy((char*)temp->value, (char*)curNode->children->content);
                  insertBack(wpt->attributes, temp);
              }
          }
        }
    }

    return wpt;
}

Attribute* createAttribute(GPXdoc* doc, xmlDoc* xml) {

  //Intializes Attributes and allocates memory
  Attribute *attr = calloc(1, sizeof(Attribute));
  if (attr == NULL) {
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }
  attr->name = calloc(500, sizeof(char));
  if (attr->name == NULL) {
    free(attr);
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }
  attr->value = calloc(500, sizeof(char));
  if (attr->value == NULL) {
    free(attr->name);
    free(attr);
    deleteGPXdoc(doc);
    xmlFreeDoc(xml);
    return NULL;
  }

  return attr;
}

int countFunction (const GPXdoc *doc, int num) {
  if (doc == NULL) {
    return 0;
  }

  int attr = 0;
  int wpt = 0;
  int rte = 0;
  int trkseg = 0;
  int trk = 0;

  Node *listNode = NULL;

//Waypoints List
  wpt += getLength(doc->waypoints);
  for (listNode = doc->waypoints->head; listNode != NULL; listNode = listNode->next) {
    Waypoint* temp = (Waypoint*)listNode->data;
    if (strcmp(temp->name, "") != 0) {
      attr++;
    }
    attr += getLength(temp->attributes);
  }

//Routes Lists
rte += getLength(doc->routes);
listNode = doc->routes->head;

for (listNode = doc->routes->head; listNode != NULL; listNode = listNode->next) {
  Route* route = (Route *)listNode->data;
  if (strcmp(route->name, "") != 0) {
    attr++;
  }
  attr += getLength(route->attributes);
  Node *node = NULL;
  for (node = route->waypoints->head; node != NULL; node = node->next) {
    Waypoint* waypoint = (Waypoint*)node->data;
    if (strcmp(waypoint->name, "") != 0) {
      attr++;
    }
    attr += getLength(waypoint->attributes);
  }
}

//Track Lists
trk += getLength(doc->tracks);

for (listNode = doc->tracks->head; listNode != NULL; listNode = listNode->next) {
  Track* track = (Track*)listNode->data;
  if (strcmp(track->name, "") != 0) {
    attr++;
  }
  attr += getLength(track->attributes);
  trkseg += getLength(track->segments);
  Node *node = NULL;
  for (node = track->segments->head; node != NULL; node = node->next) {
    TrackSegment *trackseg = (TrackSegment*)node->data;
    Node * nn = NULL;
    for(nn = trackseg->waypoints->head; nn != NULL; nn = nn ->next) {
      Waypoint *wp = (Waypoint*)nn->data;
      if (strcmp(wp->name, "") != 0) {
        attr++;
      }
      attr += getLength(wp->attributes);
    }
  }
}

if (num == 1) {
  return wpt;
}
else if (num == 2) {
  return rte;
}
else if (num == 3) {
  return trk;
}
else if (num == 4) {
  return trkseg;
}
else if (num == 5) {
  return attr;
}

  return 0;
}

//**************Assignment 2******************

xmlDocPtr convertGPX (GPXdoc* doc) {

  if (doc == NULL) {
    deleteGPXdoc(doc);
    return NULL;
  }

  LIBXML_TEST_VERSION

  xmlDocPtr xml = NULL;       // Doc pointer
  xmlNodePtr root = NULL;

  //Create the xmlDoc
  xml = xmlNewDoc(BAD_CAST "1.0");
  //Create intial/root node
  root = xmlNewNode(NULL, BAD_CAST "gpx");

  xmlDocSetRootElement(xml, root);

  char *vs = calloc(10, sizeof(float));
  sprintf(vs, "%.1f", doc->version);

  //Create namespace
  xmlSetNs(root, xmlNewNs(root, BAD_CAST "http://www.topografix.com/GPX/1/1", NULL));
  //Create version and creator
  xmlNewProp(root, BAD_CAST "version", BAD_CAST vs);
  free(vs);
  xmlNewProp(root, BAD_CAST "creator", BAD_CAST doc->creator);

  //Create Waypoints
  for (Node *listNode = doc->waypoints->head; listNode != NULL; listNode = listNode->next) {
    Waypoint* wpt = (Waypoint*)listNode->data;
    xmlNodePtr nodeW = xmlNewChild(root, NULL, BAD_CAST "wpt", NULL);
    char* lat = calloc(10, sizeof(float));
    sprintf(lat, "%f", wpt->latitude);
    xmlNewProp(nodeW, BAD_CAST "lat", BAD_CAST lat);
    free(lat);
    char* lon = calloc(10, sizeof(float));
    sprintf(lon, "%f", wpt->longitude);
    xmlNewProp(nodeW, BAD_CAST "lon", BAD_CAST lon);
    free(lon);

    if (strcmp(wpt->name, "") != 0) {
      xmlNewChild(nodeW, NULL, BAD_CAST "name", BAD_CAST wpt->name);
    }

    for (Node* n = wpt->attributes->head; n != NULL; n = n->next) {
      Attribute* attr = (Attribute*)n->data;
      xmlNewChild(nodeW, NULL, BAD_CAST attr->name, BAD_CAST attr->value);
    }
  }

  //Create Routes
  for (Node *listNode = doc->routes->head; listNode != NULL; listNode = listNode->next) {

    Route* rte = (Route*)listNode->data;
    xmlNodePtr nodeR = xmlNewChild(root, NULL, BAD_CAST "rte", NULL);

    xmlNewTextChild(nodeR, NULL, BAD_CAST "name", BAD_CAST rte->name);

    for (Node* n = rte->attributes->head; n != NULL; n = n->next) {
      Attribute* attr = (Attribute*)n->data;
      xmlNewTextChild(nodeR, NULL, BAD_CAST attr->name, BAD_CAST attr->value);
    }

    for (Node *listNode = rte->waypoints->head; listNode != NULL; listNode = listNode->next) {
      Waypoint* wpt = (Waypoint*)listNode->data;
      xmlNodePtr nodeRte = xmlNewChild(nodeR, NULL, BAD_CAST "rtept", NULL);
      char* lat = calloc(10, sizeof(float));
      sprintf(lat, "%f", wpt->latitude);
      xmlNewProp(nodeRte, BAD_CAST "lat", BAD_CAST lat);
      free(lat);
      char* lon = calloc(10, sizeof(float));
      sprintf(lon, "%f", wpt->longitude);
      xmlNewProp(nodeRte, BAD_CAST "lon", BAD_CAST lon);
      free(lon);

      if (strcmp(wpt->name, "") != 0) {
        xmlNewChild(nodeRte, NULL, BAD_CAST "name", BAD_CAST wpt->name);
      }

      for (Node* n = wpt->attributes->head; n != NULL; n = n->next) {
        Attribute* attr = (Attribute*)n->data;
        xmlNewChild(nodeRte, NULL, BAD_CAST attr->name, BAD_CAST attr->value);
      }

    }
  }

  //Create Tracks

  for(Node* listNode = doc->tracks->head; listNode != NULL; listNode = listNode->next) {
    Track* trk = (Track*)listNode->data;
    xmlNodePtr nodeT = xmlNewChild(root, NULL, BAD_CAST "trk", NULL);

    xmlNewChild(nodeT, NULL, BAD_CAST "name", BAD_CAST trk->name);

    for (Node* n = trk->attributes->head; n != NULL; n = n->next) {
      Attribute* attr = (Attribute*)n->data;
      xmlNewChild(nodeT, NULL, BAD_CAST attr->name, BAD_CAST attr->value);
    }

    for (Node* node = trk->segments->head; node != NULL; node = node->next) {
      TrackSegment* seg = (TrackSegment*)node->data;
      xmlNodePtr nodeSeg = xmlNewChild(nodeT, NULL, BAD_CAST "trkseg", NULL);

      for (Node *nn = seg->waypoints->head; nn != NULL; nn = nn->next) {
        Waypoint* wpt = (Waypoint*)nn->data;
        xmlNodePtr nodeTrkpt = xmlNewChild(nodeSeg, NULL, BAD_CAST "trkpt", NULL);
        char* lat = calloc(10, sizeof(float));
        sprintf(lat, "%f", wpt->latitude);
        xmlNewProp(nodeTrkpt, BAD_CAST "lat", BAD_CAST lat);
        free(lat);
        char* lon = calloc(10, sizeof(float));
        sprintf(lon, "%f", wpt->longitude);
        xmlNewProp(nodeTrkpt, BAD_CAST "lon", BAD_CAST lon);
        free(lon);

        if (strcmp(wpt->name, "") != 0) {
          xmlNewChild(nodeTrkpt, NULL, BAD_CAST "name", BAD_CAST wpt->name);
        }

        for (Node* n = wpt->attributes->head; n != NULL; n = n->next) {
          Attribute* attr = (Attribute*)n->data;
          xmlNewChild(nodeTrkpt, NULL, BAD_CAST attr->name, BAD_CAST attr->value);
        }
      }
    }
  }

  return xml;
}

float haversine (Waypoint* wpt1, Waypoint* wpt2) {

  double pi = 3.141592653589793;//PI
  double R = 6371000; //Earth's radius

  //Set waypoints lon and lat
  double lat1 = wpt1->latitude;
  double lon1 = wpt1->longitude;
  double lat2 = wpt2->latitude;
  double lon2 = wpt2->longitude;

  //Convert to radians
  double latR1 = lat1*(pi/180);
  double latR2 = lat2*(pi/180);
  double latDiff = (lat2 - lat1)*(pi/180);
  double lonDiff = (lon2 - lon1)*(pi/180);

  double a;
  a = sin(latDiff/2) * sin(latDiff/2) + cos(latR1) * cos(latR2) * sin(lonDiff/2) * sin(lonDiff/2);

  double c;
  c = 2 * atan2(sqrt(a), sqrt(1-a));

  double dist;
  dist = R * c;

  return dist;
}

Waypoint* intializeWaypoint(float lat, float lon) {
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
  wpt->latitude = (double)lat;
  wpt->longitude = (double)lon;

  return wpt;
}

Attribute* intializeAttr() {
  //Intializes Attributes and allocates memory
  Attribute *attr = calloc(1, sizeof(Attribute));
  if (attr == NULL) {
    return NULL;
  }
  attr->name = calloc(500, sizeof(char));
  if (attr->name == NULL) {
    free(attr);
    return NULL;
  }
  attr->value = calloc(500, sizeof(char));
  if (attr->value == NULL) {
    free(attr->name);
    free(attr);
    return NULL;
  }
  return attr;
}

Route* copyRoute(Route *route) {
  //Initialize Route
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

  strcpy(rte->name, route->name);
  //Copy input route attributes into new route
  for (Node *node = route->attributes->head; node != NULL; node = node->next) {
    Attribute *attr = (Attribute*)node->data;

    //Intialize Attribute
    Attribute *copyA = intializeAttr();
    strcpy(copyA->name, attr->name);
    strcpy(copyA->value, attr->value);
    insertBack(rte->attributes, copyA);

  }
  for (Node *n = route->waypoints->head; n != NULL; n = n->next) {
    Waypoint *wpt = (Waypoint*)n->data;

    //Intialize Waypoint
    Waypoint *copyW = intializeWaypoint(wpt->latitude, wpt->longitude);
    strcpy(copyW->name, wpt->name);
    insertBack(rte->waypoints, copyW);

    //Intialize Attributes in Waypoints
    for (Node* nn = wpt->attributes->head; nn !=NULL; nn = nn->next) {
      Attribute *attrW = (Attribute*)nn->data;

      Attribute *copyAw = intializeAttr();
      strcpy(copyAw->name, attrW->name);
      strcpy(copyAw->value, attrW->value);
      insertBack(copyW->attributes, copyAw);
    }
  }

  return rte;
}

Track* copyTrack(Track *track) {

  Track *trk = calloc(1, sizeof(Track));
  if (trk == NULL) {
    return NULL;
  }
  trk->name = calloc(500, sizeof(char));
  if (trk->name == NULL) {
    free(trk);
    return NULL;
  }

  trk->segments = initializeList(&trackSegmentToString, &deleteTrackSegment, &compareTrackSegments);
  trk->attributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

  strcpy(trk->name, track->name);

  //Copy input track attributes into new track
  for (Node *node = track->attributes->head; node != NULL; node = node->next) {
    Attribute *attr = (Attribute*)node->data;

    //Intialize Attribute
    Attribute *copyA = intializeAttr();
    strcpy(copyA->name, attr->name);
    strcpy(copyA->value, attr->value);
    insertBack(trk->attributes, copyA);
  }
  for (Node *nd = track->segments->head; nd != NULL; nd = nd->next) {
    TrackSegment *seg = (TrackSegment*)nd->data;

    TrackSegment *trkseg = calloc(1, sizeof(TrackSegment));
    if (trkseg == NULL) {
      return NULL;
    }

    trkseg->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);

    for (Node *n = seg->waypoints->head; n != NULL; n = n->next) {
      Waypoint *wpt = (Waypoint*)n->data;

      //Intialize Waypoint
      Waypoint *copyW = intializeWaypoint(wpt->latitude, wpt->longitude);
      strcpy(copyW->name, wpt->name);
      insertBack(trkseg->waypoints, copyW);

      //Intialize Attributes in Waypoints
      for (Node* nn = wpt->attributes->head; nn !=NULL; nn = nn->next) {
        Attribute *attrW = (Attribute*)nn->data;

        Attribute *copyAw = intializeAttr();
        strcpy(copyAw->name, attrW->name);
        strcpy(copyAw->value, attrW->value);
        insertBack(copyW->attributes, copyAw);
      }
    }
    insertBack(trk->segments, trkseg);
  }

  return trk;
}
