#ifndef STATION_H
#define STATION_H

typedef struct StationData {
    int id;
    int capacity;
    int consumption;
    struct StationData *next;
} StationData;

StationData *load_station_data(const char *filename);
void free_station_data(StationData *data);

#endif // STATION_H
