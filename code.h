#pragma once
#include "four.h"

#include <iostream>
#include <string>
#include <vector>

void createDSEG(); //����DSEG��
void createCSEGBEGIN(); //����CSEG��
void createCSEGEND(); //����CSEG��
void createCode(vector<QT>resQT); //����һ����Ԫʽ������һ�δ���
void code(QT temp, string lastOper, int i); //���ݵ�����Ԫʽ����CODE

