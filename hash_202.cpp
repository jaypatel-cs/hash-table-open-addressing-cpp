/*
  Name: Jay Patel
  Class: CS202 (UTK)
  Lab 5: Hash Tables (Hash_202)

  Overview:
  Implements Hash_202 using open addressing.
  Keys are hex strings, values are non-empty strings.
  Hash functions: Last7 and XOR
  Collision strategies: Linear probing and Double hashing
*/

#include "hash_202.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>

using namespace std;

/* ---------------- Helper Functions (private to this .cpp) ---------------- */

/* true if s is non-empty and every char is a hex digit */
static bool Is_All_Hex(const string &s)
{
  if (s == "") return false;

  for (size_t i = 0; i < s.size(); i++) {
    if (!isxdigit((unsigned char)s[i])) return false;
  }
  return true;
}

/* convert a hex string to an unsigned long long (assumes valid hex) */
static unsigned long long Hex_To_Ull(const string &hexstr)
{
  istringstream ss(hexstr);
  unsigned long long x = 0;
  ss >> hex >> x;
  return x;
}

/* Last7 hash: take last 7 characters (or all if shorter), interpret as hex */
static unsigned long long Hash_Last7(const string &key)
{
  if (key.size() <= 7) {
    return Hex_To_Ull(key);
  } else {
    return Hex_To_Ull(key.substr(key.size() - 7, 7));
  }
}

/* XOR hash: split into 7-char chunks from the front, XOR them as hex numbers */
static unsigned long long Hash_XOR(const string &key)
{
  unsigned long long h = 0;

  for (size_t pos = 0; pos < key.size(); pos += 7) {
    string chunk = key.substr(pos, 7);    // last chunk may be < 7 chars
    unsigned long long v = Hex_To_Ull(chunk);
    h = (h ^ v);
  }

  return h;
}

/* Use fxn_code from your header: 0 = XOR, 1 = Last7 */
static unsigned long long Compute_Hash(const string &key, int fxn_code)
{
  if (fxn_code == 1) return Hash_Last7(key);
  return Hash_XOR(key); // fxn_code == 0
}

/* ------------------------- Hash_202 Methods ------------------------- */

string Hash_202::Set_Up(size_t table_size, const string &fxn, const string &collision)
{
  // Error 1: already set up
  if (Keys.size() != 0) return "Hash table already set up";

  // Error 2: bad size
  if (table_size == 0) return "Bad table size";

  // Error 3: bad hash function
  if (fxn != "Last7" && fxn != "XOR") return "Bad hash function";

  // Error 4: bad collision strategy
  if (collision != "Linear" && collision != "Double")
    return "Bad collision resolution strategy";

  // Build empty table
  Keys.assign(table_size, "");
  Vals.assign(table_size, "");

  Nkeys = 0;
  Nprobes = 0;

  // Store hash function (your header: 0=XOR, 1=Last7)
  if (fxn == "Last7") Fxn = 1;
  else Fxn = 0;

  // Store collision strategy (your header: 'L' or 'D')
  if (collision == "Linear") Coll = 'L';
  else Coll = 'D';

  return "";
}

string Hash_202::Add(const string &key, const string &val)
{
  // Error 1: table not set up
  if (Keys.size() == 0) return "Hash table not set up";

  // Error 2: empty key
  if (key == "") return "Empty key";

  // Error 3: bad key
  if (!Is_All_Hex(key)) return "Bad key (not all hex digits)";

  // Error 4: empty val
  if (val == "") return "Empty val";

  // Error 5: full
  if (Nkeys == Keys.size()) return "Hash table full";

  size_t table_size = Keys.size();

  // First hash decides starting index
  unsigned long long h1 = Compute_Hash(key, Fxn);
  size_t start = (size_t)(h1 % table_size);

  // Step size for probing
  size_t step = 1;

  // Double hashing: use the other hash function for the step
  if (Coll == 'D') {
    int other_fxn = (Fxn == 1) ? 0 : 1;         // swap 1<->0
    unsigned long long h2 = Compute_Hash(key, other_fxn);
    step = (size_t)(h2 % table_size);
    if (step == 0) step = 1;                    // required by lab
  }

  // Probe up to table_size times (prevents infinite loops)
  for (size_t i = 0; i < table_size; i++) {
    size_t idx = (start + i * step) % table_size;

    // If key already exists
    if (Keys[idx] == key) return "Key already in the table";

    // Empty slot -> insert
    if (Keys[idx] == "") {
      Keys[idx] = key;
      Vals[idx] = val;
      Nkeys++;
      return "";
    }
  }

  // If we never found a spot, we got stuck in a cycle (double hashing case)
  return "Cannot insert key";
}

string Hash_202::Find(const string &key)
{
  // Not set up -> empty
  if (Keys.size() == 0) return "";

  // Bad key -> empty
  if (key == "") return "";
  if (!Is_All_Hex(key)) return "";

  size_t table_size = Keys.size();

  unsigned long long h1 = Compute_Hash(key, Fxn);
  size_t start = (size_t)(h1 % table_size);

  size_t step = 1;
  if (Coll == 'D') {
    int other_fxn = (Fxn == 1) ? 0 : 1;
    unsigned long long h2 = Compute_Hash(key, other_fxn);
    step = (size_t)(h2 % table_size);
    if (step == 0) step = 1;
  }

  // i is the number of "probes" (moves) we did from the start index
  for (size_t i = 0; i < table_size; i++) {
    size_t idx = (start + i * step) % table_size;

    // Empty slot means it was never inserted
    if (Keys[idx] == "") {
      Nprobes = i;
      return "";
    }

    // Found it
    if (Keys[idx] == key) {
      Nprobes = i;
      return Vals[idx];
    }
  }

  // Cycle case: never found empty or the key
  Nprobes = table_size;
  return "";
}

void Hash_202::Print() const
{
  // Do nothing if not set up
  if (Keys.size() == 0) return;

  for (size_t i = 0; i < Keys.size(); i++) {
    if (Keys[i] != "") {
      cout << setw(5) << i << " " << Keys[i] << " " << Vals[i] << endl;
    }
  }
}

size_t Hash_202::Total_Probes()
{
  // Not set up -> 0
  if (Keys.size() == 0) return 0;

  size_t total = 0;

  // For each key in the table, Find() it and add Nprobes
  for (size_t i = 0; i < Keys.size(); i++) {
    if (Keys[i] != "") {
      Find(Keys[i]);          // sets Nprobes
      total += Nprobes;
    }
  }

  return total;
}
