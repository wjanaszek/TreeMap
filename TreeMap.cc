/**
@file asd.cc

Plik do modyfikacji w w ramach cwiczenia 4 z AISDI.
Zawiera niekompletne implementacje metod klasy TreeMap.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY TreeMap.

@author
Pawel Cichocki, Michal Nowacki

@date
last revision
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: lab #4
- 2005.11.17 Michal Nowacki: lab #3, copy operator and constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <algorithm>
#include <iostream>
#include "TreeMap.h"
#include <iostream>

/// A helper class.
class TreeMapDetail //Helper
{
	friend class TreeMap;

	/// Rekurencyjna metoda pomocnicza do sprawdzania rownosci strukturalnej 2 drzew
    static bool structEqRec(TreeNode * e, TreeNode * d) {
        if((e != NULL) && (d != NULL))  {
            return structEqRec(e->left, d->left);
            if(e->data.first == d->data.first)  {
                return true;
            }
            else    {
                return false;
            }
            return structEqRec(e->right, d->right);
        }
        else if((e == NULL) && (d == NULL)) {
            return true;
        }
        else    {
            return false;
        }
    }

    /// Rekurencyjna metoda do kopiowania wezlow do drzewa m
    static void copyRec(TreeMap * m, TreeNode * d)  {
        if(d != NULL)   {
            m->insert(d->data);
            copyRec(m, d->left);
            copyRec(m, d->right);
        }
        return;
    }
};

//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

TreeMap::TreeMap()
{
	Val v;
	Key k = 32767;
	root = new TreeNode(std::make_pair(k, v));
};

/// Content of existing TreeMap object is copied into the new object.
TreeMap::TreeMap( const TreeMap& m )
{
	Val v;
	Key k = 32767;
	root = new TreeNode(std::make_pair(k, v));
	detail->copyRec(this, m.root->left);
};

TreeMap::~TreeMap()
{
	clear();
	delete root;
};

// Inserts an element into the map.
// @returns A pair whose bool component is true if an insertion was
//			made and false if the map already contained an element
//			associated with that key, and whose iterator component coresponds to
//			the address where a new element was inserted or where the element
//			was already located.
std::pair<TreeMap::iterator, bool> TreeMap::insert(const std::pair<Key, Val>& entry)
{
    TreeMap::iterator it;
	it = find(entry.first);
	if(it == end()) {
        it = unsafe_insert(entry);
		return std::make_pair(it, true);
	}
	else    {
		return std::make_pair(it, false);
	}
}

// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.

TreeMap::iterator TreeMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
	TreeNode * toAdd = new TreeNode(entry);
	TreeNode * tmp, * toAddParent;
	tmp = root;
	toAddParent = nullptr;
	while(tmp != nullptr)   {
		toAddParent = tmp;
		if(entry.first < tmp->data.first)   {
            tmp = tmp->left;
		}
		else if(entry.first > tmp->data.first)    {
            tmp = tmp->right;
		}
	}
	toAdd->parent = toAddParent;
	if(entry.first < toAddParent->data.first)   {
        toAddParent->left = toAdd;
	}
	else if(entry.first > toAddParent->data.first)   {
        toAddParent->right = toAdd;
	}
	return iterator(toAdd);
}

// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k)
{
    TreeMap::iterator it;
    for(it = begin(); it != end(); ++it)    {
        if(it.node->data.first == k)    {
            return it;
        }
    }
    return end();
}

TreeMap::const_iterator TreeMap::find(const Key& k) const
{
	TreeMap::const_iterator it;
    for(it = begin(); it != end(); ++it)    {
        if(it.node->data.first == k)    {
            return it;
        }
    }
    return end();
}

// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k)
{
	TreeMap::iterator it;
    it = find(k);
    if(it == end()) {
        Val v;
        it = unsafe_insert(std::make_pair(k, v));
        return it.node->data.second;
    }
    else    {
        return it.node->data.second;
    }
}

// Tests if a map is empty.
bool TreeMap::empty( ) const
{
	return root->left == nullptr;
}

// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size( ) const
{
    TreeMap::const_iterator it;
	size_type s = 0;
	for(it = begin(); it != end(); ++it)    {
        s++;
	}
	return s;
}

// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count(const Key& _Key) const
{
	TreeMap::const_iterator it;
    size_type c = 0;
    for(it = begin(); it != end(); ++it)    {
        if(it.node->data.first == _Key) {
            c++;
        }
    }
    return c;
}

// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator i)
{
    TreeNode * toDel;
    if(i == end())  {
        return i;
    }
    else    {
        toDel = i.node;
        ++i;
        /// 1.) Wezel jest lisciem
        if((toDel->left == nullptr) && (toDel->right == nullptr)) {
            if(toDel->parent->right == toDel)   {
                toDel->parent->right = nullptr;
            }
            if(toDel->parent->left == toDel)    {
                toDel->parent->left = nullptr;
            }
            toDel->parent = nullptr;
            delete toDel;
            return i;
        }
        /// 2.) Wezel ma jednego potomka
        else if((toDel->left != nullptr) && (toDel->right == nullptr))    {
            if(toDel->parent->right == toDel)   {
                toDel->parent->right = toDel->left;
                toDel->left->parent = toDel->parent;
            }
            if(toDel->parent->left == toDel)    {
                toDel->parent->left = toDel->left;
                toDel->left->parent = toDel->parent;
            }
            delete toDel;
            return i;
        }
        else if((toDel->left == nullptr) && (toDel->right != nullptr))    {

                if(toDel->parent->right == toDel)   {
                    toDel->parent->right = toDel->right;
                    toDel->right->parent = toDel->parent;
                }
                if(toDel->parent->left == toDel)    {
                    toDel->parent->left = toDel->right;
                    toDel->right->parent = toDel->parent;
                }
            delete toDel;
            return i;
        }
        /// 3.) Wezel ma obu potomkow
        else if((toDel->left != nullptr) && (toDel->right != nullptr))    {
            TreeNode * succ;
            succ = toDel;
            succ = succ->right;
            while(succ->left != nullptr)   {
                succ = succ->left;
            }
            toDel->data = succ->data;
            if(succ->right != nullptr) {
                succ->right->parent = succ->parent;
                if(succ->parent->right == succ) {
                    succ->parent->right = succ->right;
                }
                if(succ->parent->left == succ)  {
                    succ->parent->left = succ->right;
                }
            }
            else    {
                if(succ->parent->right == succ) {
                    if(succ->left == nullptr)  {
                        succ->parent->right = nullptr;
                    }
                }
                if(succ->parent->left == succ)  {
                    if(succ->left == nullptr)  {
                        succ->parent->left = nullptr;
                    }
                }
            }
            toDel = succ;
            delete toDel;
            return i;
        }
    }
}

// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l)
{
	while(f != l)   {
        f = erase(f);
	}
	return l;
}

// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//			Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase(const Key& key)
{
	TreeMap::iterator it;
	it = find(key);
	if(it == end()) {
        return 0;
	}
	else    {
		erase(it);
		return 1;
	}
}

// Erases all the elements of a map.
void TreeMap::clear()
{
	erase(begin(), end());
}

bool TreeMap::struct_eq(const TreeMap& another) const
{
    if(size() != another.size())    {
        return false;
    }
    else    {
        return detail->structEqRec(this->root, another.root);
    }
}

bool TreeMap::info_eq(const TreeMap& another) const
{
	TreeMap::const_iterator it, a_it;
    if(size() != another.size())    {
        return false;
    }
    for(a_it = another.begin(); a_it != another.end(); ++a_it)  {
        it = find(a_it.node->data.first);
		if(it == end()) {
            return false;
		}
	}
    return true;
}

// preincrement
TreeMap::const_iterator& TreeMap::const_iterator::operator ++()
{
	if(this->node->parent == nullptr)   {
        return * this;                  /// jestesmy w korzeniu (end) - z korzenia nie mamy nigdy prawego poddrzewa
	}
	else    {
        if(this->node->right != nullptr)    {
            this->node = this->node->right;
            while(this->node->left != nullptr)  {
                this->node = this->node->left;
            }
            return * this;
        }
        else    {
            while((this->node->parent != nullptr) && (this->node->parent->right == this->node))    {
                this->node = this->node->parent;
            }
            this->node = this->node->parent;
            return * this;
        }
    }
}

// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int)
{
	const_iterator tmp = * this;
	++(* this);
	return tmp;
}

// predecrement
TreeMap::const_iterator& TreeMap::const_iterator::operator--()
{
	if(this->node->left != nullptr) {
		this->node = this->node->left;
		while(this->node->right != nullptr) {
            this->node = this->node->right;
		}
		return * this;
	}
	else    {
		if((this->node->left == nullptr) && (this->node->parent == nullptr)) {
            return * this;          /// jestesmy w korzeniu, nic nie robimy
		}
		while((this->node->parent != nullptr) && (this->node->parent->left == this->node))   {
			this->node = this->node->parent;
		}
		if(this->node->parent == nullptr)   {
            while(this->node->left != nullptr)  {
                this->node = this->node->left;
            }
            return * this;          /// nie udalo sie znalezc poprzednika, jestesmy w korzeniu (end) - dany element byl najmniejszy, zatem musimy do niego wrocic
		}
		this->node = this->node->parent;
		return * this;
	}
}

// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int)
{
	const_iterator tmp = * this;
	--(* this);
	return tmp;
}


/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& other)
{
	if(other == * this)    {
        return * this;
	}
	else    {
        this->clear();
        this->detail->copyRec(this, other.root->left);
        return * this;
	}
}

/// Returns an iterator addressing the first element in the map
TreeMap::iterator TreeMap::begin()
{
	TreeNode * tmp = root;
	while(tmp->left != nullptr) {
        tmp = tmp->left;
	}
	return iterator(tmp);
}

TreeMap::const_iterator TreeMap::begin() const
{
	TreeNode * tmp = root;
	while(tmp->left != nullptr) {
        tmp = tmp->left;
	}
	return iterator(tmp);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end()
{
	return iterator(root);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end() const
{
	return iterator(root);
}

//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

/// A helper function that outputs a key-value pair.
void print(const std::pair<int, std::string>& p)
{
	std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// The big mean test function ;)
void test()
{
	// A typedef used by the test.
	//typedef std::map<int, std::string> TEST_MAP;
	//typedef SmallMap<int, std::string> TEST_MAP;
	typedef TreeMap TEST_MAP;

	std::cout << "Testy uzytkownika:" << std::endl;

	TEST_MAP m, * h;

	m[2] = "Merry";
    m[40] = "Jane";
    m[80] = "Korwin";
    m[1] = "Magdalena";

    h = new TreeMap(m);

    for_each(m.begin(), m.end(), print );
    //std::cout << "\nW odwroconej kolejnosci:\n";
    //for_each(m.end(), m.begin(), print);

    std::cout << "Rozmiar: " << m.size() << "\n";
    std::cout << "Kopia: " << h->size() << "\n";

    std::cout << "\nPOROWNANIE SLOWNIKA SAMEGO Z SOBA:\n";
    std::cout << "Info-eq -- h: " << h->info_eq(*h) << "\n";
    std::cout << "Info-eq podejscie drugie -- m: " << m.info_eq(m) << "\n";
    std::cout << "Struct-eq -- h: " << h->struct_eq(*h) << "\n";
    std::cout << "Struct-eq podejscie drugie -- m: " << m.struct_eq(m) << "\n";

    std::cout << "\nPOROWNANIE 2 SLOWNIKOW O TAKICH SAMYCH INFORMACJACH:\n";
    std::cout << "Info-eq: " << h->info_eq(m) << "\n";
    std::cout << "Info-eq podejscie drugie: " << m.info_eq(*h) << "\n";
    std::cout << "Struct-eq: " << h->struct_eq(m) << "\n";
    std::cout << "Struct-eq podejscie drugie: " << m.struct_eq(*h) << "\n";

    h->erase(h->find(40));

    std::cout << "\nPOROWNANIE 2 ROZNYCH SLOWNIKOW:\n";
    std::cout << "Info-eq: " << h->info_eq(m) << "\n";
    std::cout << "Info-eq podejscie drugie: " << m.info_eq(*h) << "\n";
    std::cout << "Struct-eq: " << h->struct_eq(m) << "\n";
    std::cout << "Struct-eq podejscie drugie: " << m.struct_eq(*h) << "\n";

    m.clear();

    std::cout << "\nPOROWNIANIE PUSTEGO I NIEPUSTEGO SLOWNIKA:\n";
    std::cout << "Info-eq: " << h->info_eq(m) << "\n";
    std::cout << "Info-eq podejscie drugie: " << m.info_eq(*h) << "\n";
    std::cout << "Struct-eq: " << h->struct_eq(m) << "\n";
    std::cout << "Struct-eq podejscie drugie: " << m.struct_eq(*h) << "\n";

    m.clear();
    h->clear();

    std::cout << "\nPUSTE SLOWNIKI:\n";
    std::cout << "Info-eq: " << h->info_eq(m) << "\n";
    std::cout << "Info-eq podejscie drugie: " << m.info_eq(*h) << "\n";
    std::cout << "Struct-eq: " << h->struct_eq(m) << "\n";
    std::cout << "Struct-eq podejscie drugie: " << m.struct_eq(*h) << "\n";

    m[2] = "Merry";
    m[40] = "Jane";
    m[80] = "Korwin";
    m[1] = "Magdalena";
    m[35] = "Kaka";

    std::cout << "\nTest najbardziej zlozonego usuwania:\n";
    m.erase(m.find(2));
    for_each(m.begin(), m.end(), print);
    std::cout << "\nCiag dalszy:\n";
    m.erase(m.find(35));
    for_each(m.begin(), m.end(), print);
    m.erase(m.find(40));
    std::cout << "\nCiag dalszy dalszy:\n";
    for_each(m.begin(), m.end(), print);
    std::cout << "\nPo usunieciu reszty:\n";
    m.clear();
    for_each(m.begin(), m.end(), print);

    /*std::cout << "Kopia:\n";
    for_each(h->begin(), h->end(), print );
    m.clear();
    std::cout << "Po usunieciu: " << m.size() << "\n";

    int x = 200;
    std::string a = "LUK{A0";
    //system("PAUSE");
    m.insert(std::make_pair(x, a));
    int y = 100;
    std::string b = "LUK";
    m.insert(std::make_pair(y, b));
    m.erase(m.find(30));

    for_each(m.begin(), m.end(), print );

    std::cout << "Rozmiar: " << m.size() << "\n";

    int z = 300;
    std::string c = "ZZZZ";
    m.insert(std::make_pair(z, c));

    std::cout << "\nTEST - begin\n";
    std::cout << "Rozmiar: " << m.size() << "\n";
    m.erase(m.find(200));
    std::cout << "\nTEST\n";
    std::cout << "Po usunieciu: " << m.size() << "\n";
    m.clear();

    std::cout << "Po usunieciu: " << m.size() << "\n";
    for_each(m.begin(), m.end(), print );
	//system("PAUSE");*/
	delete h;

}

//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /materialy/AISDI/z2/main.cc
//////////////////////////////////////////////////////////////////////////////

//int main()
//{
//	std::cout << "AISDI cwiczenie 4: wchodze do funkcji main." << std::endl;

//	test();
//	// Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//	Test2();
//	//system("PAUSE");
//	return EXIT_SUCCESS;
//}
