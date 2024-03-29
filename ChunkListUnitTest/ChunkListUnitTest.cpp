#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../ChunkList/ChunkList.h"

using namespace fefu_laboratory_two;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChunkListUnitTest
{
	TEST_CLASS(ConstructorTests)
	{
	public:
		TEST_METHOD(DefaultConstructorTest)
		{
			ChunkList<int, 8> list;
			Assert::IsTrue(list.empty() == true);
			Assert::IsTrue(list.size() == 0);
		}

		TEST_METHOD(CopyConstructorTest)
		{
			ChunkList<int, 8> list1;
			for (int i = 0; i < 8; i++) {
				list1.push_back(i);
			}
			auto list2 = list1;
			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(InitializerListConstructorShould)
		{
			ChunkList<int, 8> list1 = { 1,2,3,4,5 };
			ChunkList<int, 8> list2;
			for (int i = 1; i <= 5; i++) {
				list2.push_back(i);
			}
			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(InputItConstructorShould)
		{
			std::vector<int> v = { 1,2,3,4,5 };
			auto first = v.begin();
			auto last = v.end();
			ChunkList<int, 8> list1(first, last);
			ChunkList<int, 8> list2;
			for (int i = 1; i <= 5; i++) {
				list2.push_back(i);
			}
			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(AssignShould)
		{
			ChunkList<int, 8> list1;
			ChunkList<int, 8> list2;
			for (int i = 0; i < 8; i++) {
				list1.push_back(2);
			}
			list2.assign(8, 2);

			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(AssignItShould)
		{
			std::vector<int> v = { 1,2,3,4,5 };
			auto first = v.begin();
			auto last = v.end();
			ChunkList<int, 8> list1;
			list1.assignIt(first, last);
			ChunkList<int, 8> list2;
			for (int i = 1; i <= 5; i++) {
				list2.push_back(i);
			}

			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(AssignInitListShould)
		{
			ChunkList<int, 8> list1;
			list1.assign({ 1,2,3,4,5 });

			ChunkList<int, 8> list2;
			for (int i = 1; i <= 5; i++) {
				list2.push_back(i);
			}

			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(AssignmentOperatorShould)
		{
			ChunkList<int, 8> list1;
			list1 = { 1,2,3,4,5 };
			ChunkList<int, 8> list2;
			for (int i = 1; i <= 5; i++) {
				list2.push_back(i);
			}

			Assert::IsTrue(list1 == list2);
		}
	};

	TEST_CLASS(ElementAccessTests)
	{
	public:
		TEST_METHOD(AtShould)
		{
			ChunkList<int, 8> list;
			for (int i = 0; i < 30; i++) {
				list.push_back(i);
			}
			Assert::IsTrue(list.at(0) == 0);
			Assert::IsTrue(list.at(15) == 15);
			Assert::ExpectException<std::out_of_range>([]() {
				ChunkList<int, 8> l;
				l.at(31);
				});
		}

		TEST_METHOD(SquareBracketOperatorShould)
		{
			ChunkList<int, 8> list;
			for (int i = 0; i < 30; i++) {
				list.push_back(i);
			}
			Assert::IsTrue(list[0] == 0);
			Assert::IsTrue(list[15] == 15);
		}

		TEST_METHOD(FrontShould)
		{
			ChunkList<int, 8> list;
			for (int i = 0; i < 15; i++) {
				list.push_back(i);
			}
			Assert::IsTrue(list.front() == 0);
		}

		TEST_METHOD(BackShould)
		{
			ChunkList<int, 8> list;
			for (int i = 0; i < 15; i++) {
				list.push_back(i);
			}
			Assert::IsTrue(list.back() == 14);
		}
	};

	TEST_CLASS(IteratorTests) {
		TEST_METHOD(IteratorsShould) {
			ChunkList<int, 8> list;
			int* arr = new int[15];
			for (int i = 0; i < 15; i++) {
				list.push_back(i);
				arr[i] = i;
			}

			int j = 0;
			for (auto e : list)
				Assert::IsTrue(arr[j++] == e);

			auto it1 = list.begin();
			auto it2 = list.begin();
			Assert::IsTrue(it1 == it2);
			Assert::IsTrue(it1 >= it2);
			it2++;
			Assert::IsTrue(it2 > it1);
			it1 += 3;
			Assert::IsTrue(it1 > it2);
		}
	};

	TEST_CLASS(CapacityTests) {
		TEST_METHOD(CapacityMethodsShould) {
			ChunkList<int, 8> list;
			Assert::IsTrue(list.empty() == true);
			Assert::IsTrue(list.size() == 0);
			Assert::IsTrue(list.max_size() == 0);
			for (int i = 0; i < 10; i++) {
				list.push_back(i);
			}
			Assert::IsTrue(list.size() == 10);
			Assert::IsTrue(list.max_size() == 16);
		}
	};

	TEST_CLASS(ModifierTests) {
		TEST_METHOD(EmplaceShould)
		{
			ChunkList<int, 8> list = { 1,2,3,4,5 };
			ChunkList<int, 8> list1 = { 1,2,3,23,213,123,4,5 };
			ChunkList<int, 8> list2 = { 1,2,3,23,213,123,4,5,42,231 };
			ChunkList<int, 8> list3 = { 51,654,1,2,3,23,213,123,4,5,42,231 };

			auto it = list.emplace(list.cbegin() + 3, 123, 213, 23);
			Assert::IsTrue(list == list1);

			auto it1 = list.emplace_back(42, 231);
			Assert::IsTrue(list == list2);

			auto it2 = list.emplace_front(654, 51);
			Assert::IsTrue(list == list3);
		}

		TEST_METHOD(InsertShould1) {
			ChunkList<int, 8> list;
			ChunkList<int, 8> list_should;
			list_should.push_back(100);
			list_should.push_back(0);
			list_should.push_back(1);
			list_should.push_back(2);

			for (int i = 0; i < 10; i++)
				list.push_back(i);

			list.clear();

			Assert::IsTrue(list.empty() == true);
			Assert::IsTrue(list.size() == 0);
			Assert::IsTrue(list.max_size() == 0);

			for (int i = 0; i < 3; i++)
				list.push_back(i);

			auto it = list.cbegin();
			list.insert(it, 100);

			Assert::IsTrue(list == list_should);
		}

		TEST_METHOD(InsertShould2) {
			std::vector<int> vec = { 123,54,23 };
			ChunkList<int, 8> list1 = { 1,2,3,4,5 };
			ChunkList<int, 8> list2 = { 1,2,3,4,5 };
			ChunkList<int, 8> list_should = { 23,54,123,1,2,3,4,5 };

			list1.insertIt(list1.cbegin(), vec.begin(), vec.end());

			Assert::IsTrue(list1 == list_should);

			list2.insert(list2.cbegin(), { 123,54,23 });

			Assert::IsTrue(list2 == list_should);
		}

		TEST_METHOD(ResizeShould)
		{
			ChunkList<int, 8> list1;
			list1 = { 1,2,3,4,5 };
			list1.resize(8, 100);
			ChunkList<int, 8> list2;

			for (int i = 1; i <= 5; i++)
				list2.push_back(i);
			for (int i = 0; i < 3; i++)
				list2.push_back(100);

			Assert::IsTrue(list1 == list2);

			list1.resize(4);
			list2.resize(4);

			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(EraseShould) {
			ChunkList<int, 8> list;
			ChunkList<int, 8> list_should1;
			ChunkList<int, 8> list_should2;

			for (int i = 1; i < 8; i++)
				list_should1.push_back(i);

			list_should2.push_back(1);
			list_should2.push_back(2);
			list_should2.push_back(6);
			list_should2.push_back(7);

			for (int i = 0; i < 8; i++)
				list.push_back(i);

			list.erase(list.cbegin());

			Assert::IsTrue(list == list_should1);

			auto range_begin = list.cbegin();
			auto range_end = list.cbegin();
			range_begin++;
			range_end += 4;

			list.erase(range_begin, range_end);

			Assert::IsTrue(list == list_should2);
		}

		TEST_METHOD(PushAndPopShould) {
			ChunkList<int, 8> list;

			for (int i = 0; i < 8; i++)
				list.push_back(i);

			Assert::IsTrue(list[0] == 0);
			Assert::IsTrue(list[7] == 7);

			list.pop_back();
			Assert::IsTrue(list.size() == 7);

			list.push_front(350);
			Assert::IsTrue(list[0] == 350);

			list.pop_front();
			Assert::IsTrue(list[0] == 0);
		}
	};

	TEST_CLASS(ComparisionsTests) {
		TEST_METHOD(EqualsShould) {
			ChunkList<int, 8> list1 = { 1,2,3,4,5 };
			ChunkList<int, 8> list2 = { 1,2,3,4,5 };
			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(ComparisionsShould) {
			ChunkList<int, 8> list1 = { 1,2,3,4,5 };
			ChunkList<int, 8> list2 = { 1,2,3,4,5 };
			ChunkList<int, 8> list3 = { 1,2,10,4,5 };
			ChunkList<int, 8> list4 = { 1,2,3,4,5,1 };


			Assert::IsTrue(list1 <= list2);
			Assert::IsTrue(list1 >= list2);
			Assert::IsTrue(list3 > list2);
			Assert::IsTrue(list4 > list2);
			Assert::IsTrue(list1 < list4);
			Assert::IsFalse(list2 > list3);
		}
	};
}
