#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct event {
    char *event_name;
    int start_hour;
    int start_minute;
    int end_hour;
    int end_minute;
    bool start_am;
    bool end_am;
    int day;
};

void print_event_name(struct event *current_event) {
    printf("%-25s",current_event->event_name);
}

void print_event(struct event *cur) {
    char start_string[3];
    if (cur->start_am) {
        strcpy(start_string,"am");
    } else {
        strcpy(start_string,"pm");
    }

    char end_string[3];
    if (cur->end_am) {
        strcpy(end_string,"am");
    } else {
        strcpy(end_string,"pm");
    }

    char day[10];
    switch(cur->day) {
        case 1:
            strcpy(day,"Monday");
            break;
        case 2:
            strcpy(day,"Tuesday");
            break;
        case 3:
            strcpy(day,"Wednesday");
            break;
        case 4:
            strcpy(day,"Thursday");
            break;
        case 5:
            strcpy(day,"Friday");
            break;
        case 6:
            strcpy(day,"Saturday");
            break;
        case 7:
            strcpy(day,"Sunday");
            break;
        default:
            strcpy(day,"Monday");
            break;
    }
    printf("%-25s happening from %d:%d0%2s to %d:%d0%2s on %s\n", cur->event_name, cur->start_hour, cur->start_minute/10, start_string, cur->end_hour, cur->end_minute/10, end_string, day);
}

void print_calendar(struct event week_calendar[29][7]) {
    printf("Time      %-25s%-25s%-25s%-25s%-25s%-25s%-25s\n","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday");
    char times[29][6] = {
        "8:00",
        "8:30",
        "9:00",
        "9:30",
        "10:00",
        "10:30",
        "11:00",
        "11:30",
        "12:00",
        "12:30",
        "1:00",
        "1:30",
        "2:00",
        "2:30",
        "3:00",
        "3:30",
        "4:00",
        "4:30",
        "5:00",
        "5:30",
        "6:00",
        "6:30",
        "7:00",
        "7:30",
        "8:00",
        "8:30",
        "9:00",
        "9:30",
        "10:00"
    };
    for (int r = 0; r < 29; r++) {
        printf("%-10s",times[r]);
        for (int c = 0; c < 7; c++) {
            print_event_name(&week_calendar[r][c]);
        }
        printf("\n");
    }
}

bool valid_event(struct event *cur) {
    if (!cur->start_am && cur->end_am) {
        return false;
    } else if (cur->day > 7 || cur->day < 1) {
        return false;
    } else if (cur->start_minute < 0 || cur->start_minute > 59 ||
               cur->end_minute < 0 || cur->end_minute > 59) {
        return false;           
    } else if ((cur->start_am && (cur->start_hour < 8 || cur->start_hour > 11)) ||
               (cur->start_hour > 10 && cur->start_hour != 12)) {
        return false;
    } else if (!cur->start_am && cur->start_hour == 10) {
        return false;
    } else if ((cur->end_am && (cur->end_hour < 8 || cur->end_hour > 11)) ||
               (cur->end_hour > 10 && cur->end_hour != 12)) {
        return false;
        //At this point, the times are validly inserted.
        //Now have to check that start time is before end time.
    } else if (cur->start_am == cur->end_am) {
        //I already checked for if the end time is am and the start time is pm, so 
        //the only cases I have to check are if the ams and pms match, since if start
        //is am and end is pm, time is instantly valid
        if (cur->start_hour > cur->end_hour && cur->start_hour != 12) {
            return false;
        } else if ((cur->start_hour == cur->end_hour) && (cur->start_minute >= cur->end_minute)) {
            return false;
        }
    } 
    return true;
}

void adjust_event(struct event *cur) {
    if (!cur->end_am && cur->end_hour == 10) {
        cur->end_minute = 0;
    }
    int start_min = (cur->start_minute / 30 ) * 30;
    int end_min = (cur->end_minute / 30 ) * 30;
    cur->start_minute = start_min;
    cur->end_minute = end_min;
}

int num_intervals(struct event *cur) {
    int intervals = 0;
    if (cur->start_am == cur->end_am) {
        intervals = (cur->end_hour - cur->start_hour) * 2;
        if (cur->start_hour == 12) {
            intervals = cur->end_hour * 2;
        }
    } else {
        intervals += (12 - cur->start_hour) * 2;
        intervals += (cur->end_hour) * 2;
    }
    if(cur->end_minute > 0){
            intervals++;
        }
    if(cur->start_minute > 0){
        intervals--;
    }
    return intervals;
}

void insert_event(struct event week_calendar[29][7], struct event *cur) {
    int column  = cur->day - 1;
    int row = 0;
    if (cur->start_am) {
        row += (cur->start_hour - 8) * 2;
    } else {
        if (cur->start_hour == 12) {
            row = 8;
        } else {
            row = cur->start_hour * 2;
            row += 8;
        }
    }
    if (cur->start_minute > 0) {
        row++;
    }
    int count = num_intervals(cur);
    for (int i = row; i < count + row; i++) {
        struct event temp = {
            .day = cur->day,
            .event_name = cur->event_name,
            .start_am = cur->start_am,
            .start_hour = cur->start_hour,
            .start_minute = cur->start_minute,
            .end_am = cur->end_am,
            .end_hour = cur->end_hour,
            .end_minute = cur->end_minute
        };
        if (strcmp(week_calendar[i][column].event_name, "EMPTY SLOT") != 0) {
            temp.event_name = malloc(strlen(week_calendar[i][column].event_name) + strlen(" and ") + strlen(cur->event_name) + 1);
            strcpy(temp.event_name, week_calendar[i][column].event_name);
            strcat(temp.event_name, " and ");
            strcat(temp.event_name, cur->event_name);
        }
        week_calendar[i][column] = temp;
    }
}

void generate_calendar() {
    struct event week_calendar[29][7];
    for (int r = 0; r < 29; r++) {
        for (int c = 0; c < 7; c++) {
            struct event temp = {
                .event_name = "EMPTY SLOT",
                .start_hour = 0,
                .start_minute = 0,
                .end_hour = 0,
                .end_minute = 0,
                .start_am = true,
                .end_am = true
            };
            week_calendar[r][c] = temp;
        }
    }
    bool more = true;
    do {
        char temp_name[25];
        printf("Enter an event name to add it to the schedule. (STOP to stop)\n");
        scanf("%24s", &temp_name);
        if (strcmp(temp_name, "STOP") != 0) {
            more = true;
            struct event temp;
            temp.event_name = malloc(strlen(temp_name)+1);
            strcpy(temp.event_name, temp_name);
            printf("When does the event start? (Enter as 00:00am)\n");
            char ampm[3];
            scanf("%d:%d%s",&temp.start_hour, &temp.start_minute, &ampm);
            if (strcmp(ampm, "am") == 0) {
                temp.start_am = true;
            } else {
                temp.start_am = false;
            }
            printf("When does the event end? (Enter as 00:00pm)\n");
            scanf("%d:%d%s",&temp.end_hour, &temp.end_minute, &ampm);
            if (strcmp(ampm, "am") == 0) {
                temp.end_am = true;
            } else {
                temp.end_am = false;
            }
            printf("Enter the day this event happens as 1 - 7  for Monday  - Sunday.\n");
            scanf("%d",&temp.day);
            if (!valid_event(&temp)) {
                printf("Event invalid. Please try again.\n");
            } else {
                printf("Event succesfully generated as following:\n");
                print_event(&temp);
                adjust_event(&temp);
                // printf("%d intervals\n",num_intervals(&temp));
                insert_event(week_calendar,&temp);
            }
        } else {
            more = false;
        }
    } while (more);
    print_calendar(week_calendar);
}

int main() {
    generate_calendar();
    printf("Press and key and then enter to close output.");
    scanf("%s");
    return EXIT_SUCCESS;
}