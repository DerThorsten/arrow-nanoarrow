<!---
  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing,
  software distributed under the License is distributed on an
  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
  KIND, either express or implied.  See the License for the
  specific language governing permissions and limitations
  under the License.
-->

<!-- Render with jupyter nbconvert --to markdown README.ipynb -->

# nanoarrow for Python

The nanoarrow Python package provides bindings to the nanoarrow C library. Like
the nanoarrow C library, it provides tools to facilitate the use of the
[Arrow C Data](https://arrow.apache.org/docs/format/CDataInterface.html) 
and [Arrow C Stream](https://arrow.apache.org/docs/format/CStreamInterface.html) 
interfaces.

## Installation

Python bindings for nanoarrow are not yet available on PyPI. You can install via
URL (requires a C compiler):

```bash
python -m pip install "https://github.com/apache/arrow-nanoarrow/archive/refs/heads/main.zip#egg=nanoarrow&subdirectory=python"
```

If you can import the namespace, you're good to go!


```python
import nanoarrow as na
```

## Example

The Arrow C Data and Arrow C Stream interfaces are comprised of three structures: the `ArrowSchema` which represents a data type of an array, the `ArrowArray` which represents the values of an array, and an `ArrowArrayStream`, which represents zero or more `ArrowArray`s with a common `ArrowSchema`. All three can be wrapped by Python objects using the nanoarrow Python package.

### Schemas

Use `nanoarrow.schema()` to convert a data type-like object to an `ArrowSchema`. This is currently only implemented for pyarrow objects.


```python
import pyarrow as pa
schema = na.schema(pa.decimal128(10, 3))
```

You can extract the fields of a `Schema` object one at a time or parse it into a view to extract deserialized parameters.


```python
print(schema.format)
print(schema.view().decimal_precision)
print(schema.view().decimal_scale)
```

    d:10,3
    10
    3


The `nanoarrow.schema()` helper is currently only implemented for pyarrow objects. If your data type has an `_export_to_c()`-like function, you can get the address of a freshly-allocated `ArrowSchema` as well:


```python
schema = na.Schema.allocate()
pa.int32()._export_to_c(schema._addr())
schema.view().type
```




    'int32'



The `Schema` object cleans up after itself: when the object is deleted, the underlying `Schema` is released.

### Arrays

You can use `nanoarrow.array()` to convert an array-like object to a `nanoarrow.Array`, optionally attaching a `Schema` that can be used to interpret its contents. This is currently only implemented for pyarrow objects.


```python
array = na.array(pa.array(["one", "two", "three", None]))
```

Like the `Schema`, you can inspect an `Array` by extracting fields individually:


```python
print(array.length)
print(array.null_count)
```

    4
    1


...and parse the `Array`/`Schema` combination into a view whose contents is more readily accessible.


```python
import numpy as np
view = array.view()
[np.array(buffer) for buffer in view.buffers]
```




    [array([7], dtype=uint8),
     array([ 0,  3,  6, 11, 11], dtype=int32),
     array([b'o', b'n', b'e', b't', b'w', b'o', b't', b'h', b'r', b'e', b'e'],
           dtype='|S1')]



Like the `Schema`, you can allocate an empty one and access its address with `_addr()` to pass to other array-exporting functions.


```python
array = na.Array.allocate(na.Schema.allocate())
pa.array([1, 2, 3])._export_to_c(array._addr(), array.schema._addr())
array.length
```




    3



### Array streams

You can use `nanoarrow.array_stream()` to convert an object representing a sequence of `Array`s with a common `Schema` to a `nanoarrow.ArrayStream`. This is currently only implemented for pyarrow objects.


```python
pa_array_child = pa.array([1, 2, 3], pa.int32())
pa_array = pa.record_batch([pa_array_child], names=["some_column"])
reader = pa.RecordBatchReader.from_batches(pa_array.schema, [pa_array])
array_stream = na.array_stream(reader)
```

You can pull the next array from the stream using `.get_next()` or use it like an interator. The `.get_next()` method will return `None` when there are no more arrays in the stream.


```python
print(array_stream.get_schema())

for array in array_stream:
    print(array.length)

print(array_stream.get_next() is None)
```

    struct<some_column: int32>
    3
    True


You can also get the address of a freshly-allocated stream to pass to a suitable exporting function:


```python
array_stream = na.ArrayStream.allocate()
reader._export_to_c(array_stream._addr())
array_stream.get_schema()
```




    struct<some_column: int32>



## Development

Python bindings for nanoarrow are managed with [setuptools](https://setuptools.pypa.io/en/latest/index.html).
This means you can build the project using:

```shell
git clone https://github.com/apache/arrow-nanoarrow.git
cd arrow-nanoarrow/python
pip install -e .
```

Tests use [pytest](https://docs.pytest.org/):

```shell
# Install dependencies
pip install -e .[test]

# Run tests
pytest -vvx
```
