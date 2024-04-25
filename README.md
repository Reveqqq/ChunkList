# ChunkList
## Description

Works like std::list <br>
Allocates memory for elements in blocks with a size of N elements (template parameter). <br>
When inserting, it tries to insert into existing blocks, if it cannot, adds a new one. <br>
When deleting an element, if the block becomes empty, it deletes it. <br>
Otherwise, it shifts all the elements inside the block. That is, each chunk may not be fully filled. <br>

## Features
<li>Implemented custom allocator and iterator.</li>

# ChunkListUnitTest

Unit tests for basic operations with ChunkList
