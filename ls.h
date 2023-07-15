#ifndef _LS_H
#define _LS_H

bool isHidden(struct dirent *contents);
bool isDir(struct stat fileStat);
bool isExecutable(struct stat fileStat);
void printName(struct dirent *contents);
void printInformation(struct stat fileStat, struct dirent *contents);
void print();
void ls();

#endif