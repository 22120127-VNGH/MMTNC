#include <iostream>
#include <vector>

using namespace std;

struct hashTable
{
	struct hashNode
	{
		string key;
		int value;
	};

	int capacity;
	int prime;
	vector<hashNode*> table;

	bool isPrime(int n)
	{
		if (n <= 1)
		{
			return false;
		}

		for (int i = 2; i * i <= n; i++)
		{
			if (n % i == 0)
			{
				return false;
			}
		}
		return true;
	}

	int findPrime(int n)
	{
		for (int i = n - 1; i >= 2; i--)
		{
			if (isPrime(i))
			{
				return i;
			}
		}
		return 2;
	}
	void init(unsigned int hashSize)
	{
		capacity = hashSize;
		prime = findPrime(hashSize);
		table.resize(capacity, nullptr);
	}

	void release()
	{
		for (int i = 0; i < table.size(); i++)
		{
			delete table[i];
		}
		table.clear();
	}

	unsigned int hashFunction(string key)
	{
		const int p = 31;
		const int m = 1e9 + 9;
		unsigned int hash = 0;
		unsigned int p_pow = 1;
		for (int i = 0; i < key.size(); i++)
		{
			hash = (hash + (key[i] * p_pow) % m) % m;
			p_pow = (p_pow * p) % m;
		}
		return hash % capacity;
	}

	unsigned int secondHashFunction(unsigned int hash)
	{
		return hash % prime;
	}

	void add(string key, int value)
	{
		unsigned int hash = hashFunction(key);
		unsigned int stepSize = secondHashFunction(hash);

		for (int i = 0; i < capacity; i++)
		{
			unsigned int index = (hash + i * stepSize) % capacity;

			if (table[index] == nullptr || table[index]->key == key)
			{
				if (table[index] == nullptr)
				{
					table[index] = new hashNode;
				}
				table[index]->key = key;
				table[index]->value = value;
				return;
			}
		}
	}

	int* searchValue(string key)
	{
		unsigned int hash = hashFunction(key);
		unsigned int stepSize = secondHashFunction(hash);

		for (int i = 0; i < capacity; i++)
		{
			unsigned int index = (hash + i * stepSize) % capacity;

			if (table[index] == nullptr)
			{
				return nullptr;
			}
			if (table[index]->key == key)
			{
				return &table[index]->value;
			}
		}
		return nullptr;
	}

	void removeKey(string key)
	{
		unsigned int hash = hashFunction(key);
		unsigned int stepSize = secondHashFunction(hash);

		for (int i = 0; i < capacity; i++)
		{
			unsigned int index = (hash + i * stepSize) % capacity;

			if (table[index] == nullptr)
			{
				return;
			}
			if (table[index]->key == key)
			{
				delete table[index];
				table[index] = nullptr;
				return;
			}
		}
	}
};

int main()
{
	hashTable ht;
	ht.init(10);

	ht.add("key1", 1);
	ht.add("key2", 2);
	ht.add("key3", 3);
	ht.add("key4", 4);
	ht.add("key5", 5);

	string fkey = "key5";
	int* value = ht.searchValue(fkey);

	if (value != nullptr)
	{
		cout << "Value: " << *value << "\n";
	}
	else
	{
		cout << "Key not found\n";
	}

	ht.removeKey(fkey);

	value = ht.searchValue(fkey);
	cout << "\nRemoving...\n\n";

	if (value != nullptr)
	{
		cout << "Value: " << *value << "\n";
	}
	else
	{
		cout << "Key not found" << "\n";
	}

	ht.release();

	return 0;
}