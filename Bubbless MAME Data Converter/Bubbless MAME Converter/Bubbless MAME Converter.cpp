﻿#include "pch.h"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	uint8_t BubblessPageBuffer[128] = { 0 };
	uint8_t MamePageOutput[144] = { 0 };
	uint8_t PageNumberBuffer[2] = { 0 };
	uint8_t tempCarry = 0x0;
	int option = 0;

	cout << "[Bubbless - MAME Data Converter V1.0]" << endl;
	cout << "IKA Victor Co., Inc.                  Twitter @RCAVictorCo" << endl;

	while (option != 3)
	{
	START:
		cout << "\n" << "Type an option" << endl;
		cout << "1. Bubbless to MAME" << endl;
		cout << "   Place a Bubbless file(bubbless.bin) you want to convert in the same directory." << endl;
		cout << "   This option will convert the source data to MAME format and generate the result." << endl;
		cout << "2. MAME to Bubbless" << endl;
		cout << "   Not yet supported" << endl;
		cout << "3. Exit" << endl;
		cout << endl;
		cin >> option;

		if (option == 1)
		{
			ifstream input("bubbless.bin", ios::in | ios::binary);

			if (input)
			{
				ofstream output("converted.bin", ios::out | ios::binary);

				cout << "\n" << "This routine shifts all FBM54DB pages 6 bits right and attach a page number." << endl;
				cout << "Now converting Bubbless page    ";

				//2053개 전 페이지+0x000페이지 변환
				for (uint16_t pageNumber = 0; pageNumber < 2054; pageNumber++)
				{
					//Bubbless ROM의 페이지 시작점 지정
					tempCarry = 0;
					input.seekg(2048 + (pageNumber * 128), ios::beg);
					input.read((char*)BubblessPageBuffer, sizeof(BubblessPageBuffer));

					cout << "\b\b\b";
					cout.width(3); cout.fill('0');
					cout << hex << pageNumber;

					//비트 쉬프팅 루틴
					for (int pageByte = 128; pageByte > 0; pageByte--)
					{
						tempCarry = BubblessPageBuffer[pageByte - 1] << 2; //위로 올라갈 6비트 캐리 취득
						MamePageOutput[pageByte] = MamePageOutput[pageByte] | tempCarry; //다음 바이트에 캐리 올리기
						MamePageOutput[pageByte - 1] = BubblessPageBuffer[pageByte - 1] >> 6; //기존 데이터 6비트 오른쪽 쉬프트
						tempCarry = 0x0;
					}

					//이거 없으면 이상하게 버그가 남(마지막 바이트 재작업)
					tempCarry = BubblessPageBuffer[127] << 2; //위로 올라갈 6비트 캐리 취득
					MamePageOutput[128] = tempCarry; //다음 바이트에 캐리 올리기
					tempCarry = 0x0;

					//페이지 번호 취득
					PageNumberBuffer[0] = uint8_t(pageNumber >> 8); //페이지 번호 상위 바이트
					PageNumberBuffer[1] = uint8_t(pageNumber); //페이지 번호 하위 바이트

					//페이지 시작점으로 포인터 이동 후 기록
					output.seekp(pageNumber * 144, ios::beg);
					output.write((char*)MamePageOutput, sizeof(MamePageOutput));
					//포인터 14칸 뒤로 이동 후 페이지 번호 기록
					output.seekp(-14, ios::end);
					output.write((char*)PageNumberBuffer, sizeof(PageNumberBuffer));
				}

				input.close();
				output.close();

				cout << "\n" << "Done!" << endl;

				option = 0;
				goto START;
			}
			else
			{
				cout << "\n" << "No file!" << endl;

				option = 0;
				goto START;
			}
		}
		else if (option == 2)
		{
			cout << "\n" << "Not yet supported!" << endl;

			option = 0;
			goto START;
		}
		else if (option == 3)
		{
			cout << "\n" << "Quitting..." << endl;
		}
		else
		{
			cout << "Invalid input!" << endl;

			option = 0;
			goto START;
		}
	}

	return 0;
}