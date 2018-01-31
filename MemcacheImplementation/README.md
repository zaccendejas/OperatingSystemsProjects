CSE.30341.FA17: Project 05
==========================

This is the documentation for [Project 05] of [CSE.30341.FA17].

Members
-------

1. Zac Cendejas (zcendeja@nd.edu)
2. Will Fritz (wfritz@nd.edu)

Design
------

> 1. In the `HTCache` constructor, you will need to determine the following
>    values: `Addresses`, `Pages`, `VPNShift`, `VPNMask`, and `OffsetMask`.
>    How would you determine these values and what would they be used for?

In order to determine the value for 'Adresses' we will use the value that the user gives for AddressLength. We will then use this value to calculate 'Adresses' in the following way: Addresses = 2^AddressLength. This will be used as the number of entires in our cache.

In order to determin ethe value for 'Pages' we will use the previously calculated 'Addresses' value along with the user given PageSize value. 'Pages' will be calculated in the following way: Pages = Addresses / PageSize. This will determine how big our 'PageTable' is. 

For the VPNShift you want to take the log[base 2] of the value 'PageSize'. This will be used to shift the bit representation of the hashed key after the mask has been applied to it so that it is the correct index value for the corresponding page of the hash.

The OffsetMask will be set equal to the bit represntation of ( (1 << VPNShift) - 1 ). This will be used to determine the correct offset index value for an entry given a hashed key. 

The VPNMask will be set equal to the bit representation of ( (1 << Adresses) - 1 - OffsetMask). This will be used to determine the correct VPN index value for a page given a hashed key.


> 2. In the `HTCache::get` method, you will need to determine the **VPN** and
>    **offset** based on the specified **key**.
> 
>       - How would you determine these values?
>       - How would you use these values to retrieve the specified value?
>       - What data would you need to update?
>       - What data would you need to synchronize?

Given the key we would generate a hash number. We will then mod this hash number by AddressLength so that it falls within the correct range. This will be ref3ered to as key from now on.  To find VPN we use the following calculation: 
VPN = ((key&VPNMask) >> VPNShift). To find the offset we use: offset = (key&OffsetMask).
We would use the VPN to access the correct Page in 'PageTable'. Then within that page, we would use offset to determine the index to use with the vector 'Entries'. Then we check if the key given is the key inside that entry. Other wise you look at the next entry until 1) you find the value, then return it 2) you find a NULL space, then call handler function to emplace the value there and 3) you find the original entry, then we have to evict something and emplace the new value where that element was evicted. 
We have to update ourt hits and misses. The data structures used within our plolicies. And then the values and keys in the cache itself
The Cache data and the data structures within our policies

> 3. In the `HTCache::put` method, you will need to determine the **VPN** and
>    **offset** based on the specified **key**.
> 
>       - How would you determine these values?
>       - How would you use these values to store the specified value?
>       - What data would you need to update?
>       - What data would you need to synchronize?

Given the key we would generate a hash number. We will then mod this hash number by Addresses so that it falls within the correct range. This will be ref3ered to as key from now on.  To find VPN we use the following calculation: 
VPN = ((key&VPNMask) >> VPNShift). To find the offset we use: offset = (key&OffsetMask).
We would use the VPN to access the correct Page in 'PageTable'. Then within that page, we would use offset to determine the index to use with vector 'Entries'. At that index, if it is a NULL space, then we emplace the key value pair there. If there is a key-value pair there then we check if it has the same key passed in by the user. If it is the same then that value is updated. IF not then we move to the next element in the vector. We continue to do this until we loop through the entire. Wehn whis happens and no NULL r spot witha a matching key is found, then the eviction policy is evoked and the new value is placed wherever the evicted element was. 
The data structures wihtin the policies and the cache itself.
Cache data and data structures within our policies

> 3. In the `HTPage` class, you will need to implement FIFO, Random, LRU, and
>    Clock eviction policies.  For each of these policies:
> 
>       - What bookkeeping do you need to implement the policy?
>       - How would you use this bookkeeping to implement the policy?

**FIFO**: For each entry we will track when it was creaeted. This is done by adding the offset index to a queue and popping from this queue when evicting.

**Random**: No bookeeping needs to be done for this policy. All that needs to happen is when the eviction policy is called, a random number is generated and used to randomly select an element to be removed.

**LRU**: The bookeeping for this is a vector to hold the offset indexes. Whenever an offset is used we remove it from the vector and push it to the back. This way the LRU is always at the front of the vector.

**Clock**: The bookeeping needed for this is a simple bit value for each entry. This will be set to 1 whenever an item is added. Then when something needs to get replaced: traverse the entries, and whenever the value is 1, set the value to 0. The first item you find that is 0 is the element that is removed. We also have to bookeep an iterator pointing to the last page entry accessed by this policy. This is where the search begins next time. The bit is set to 1 whenever an enrty is inserted or updated.

> 5. In the `HTPage::get`, you must use linear probing to locate an appropriate
>    `HTPageEntry`.
> 
>       - How would you determine if you found the correct `HTPageEntry`?

The correct entry is determined by whether its key matches the key provided by user. If curremt element does not, then the next element in the array is accesed. until either NULL, the first key, or the correct key is found

> 6. In the `HTPage::put`, you must use linear probing to locate an appropriate
>    `HTPageEntry`.
> 
>       - How would you determine if you need to update a value rather than add a new one?
>       - How would you determine if you need to perform an eviction?
>       - How would you perform an eviction?

We update a value when either the key matches the key at the fist location found in the array. If this is the case the value is updated. If the there is a NULL key, then a new value is added. If there is a key that does not match, we traverese the vector until either NULL, a matching key, or the first key is found. We need to perform an eviction when the whole vector is searched and either a NULL spot nor a matching key could be found. The eviction is then performed according to selected policy. Then, the new element is inserted at the spot in the vector where the element was previously evicted.

Demonstration
-------------

> Place a link to your demonstration slides on [Google Drive].
https://docs.google.com/a/nd.edu/presentation/d/1-5JGAJ8SILeIdtBcGZ4XE_68morKZVawofBQyscZb6A/edit?usp=sharing


[Project 05]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project05.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://docs.google.com/a/nd.edu/presentation/d/1-5JGAJ8SILeIdtBcGZ4XE_68morKZVawofBQyscZb6A/edit?usp=sharing
