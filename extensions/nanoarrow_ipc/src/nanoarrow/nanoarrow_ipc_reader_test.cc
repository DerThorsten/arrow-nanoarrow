// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include <gtest/gtest.h>

#include <stdio.h>

#include "nanoarrow_ipc.h"

static uint8_t kSimpleSchema[] = {
    0xff, 0xff, 0xff, 0xff, 0x10, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0a, 0x00, 0x0e, 0x00, 0x06, 0x00, 0x05, 0x00, 0x08, 0x00, 0x0a, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x0c, 0x00,
    0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x84, 0xff,
    0xff, 0xff, 0x18, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
    0x73, 0x6f, 0x6d, 0x65, 0x5f, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x00, 0x00, 0x08, 0x00,
    0x00, 0x00, 0x73, 0x6f, 0x6d, 0x65, 0x5f, 0x6b, 0x65, 0x79, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x18, 0x00,
    0x08, 0x00, 0x06, 0x00, 0x07, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x10, 0x00, 0x14, 0x00,
    0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x14, 0x00, 0x00, 0x00, 0x70, 0x00,
    0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x73, 0x6f, 0x6d, 0x65, 0x5f, 0x63, 0x6f, 0x6c, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0c, 0x00,
    0x04, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x04, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x73, 0x6f, 0x6d, 0x65, 0x5f, 0x76, 0x61, 0x6c,
    0x75, 0x65, 0x5f, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00,
    0x00, 0x00, 0x73, 0x6f, 0x6d, 0x65, 0x5f, 0x6b, 0x65, 0x79, 0x5f, 0x66, 0x69, 0x65,
    0x6c, 0x64, 0x00, 0x00, 0x08, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x07, 0x00, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t kSimpleRecordBatch[] = {
    0xff, 0xff, 0xff, 0xff, 0x88, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0c, 0x00, 0x16, 0x00, 0x06, 0x00, 0x05, 0x00, 0x08, 0x00, 0x0c, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x18, 0x00, 0x0c, 0x00,
    0x04, 0x00, 0x08, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t kEndOfStream[] = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};

TEST(NanoarrowIpcReader, InputStreamBuffer) {
  uint8_t input_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  struct ArrowBuffer input;
  ArrowBufferInit(&input);
  ASSERT_EQ(ArrowBufferAppend(&input, input_data, sizeof(input_data)), NANOARROW_OK);

  struct ArrowIpcInputStream stream;
  uint8_t output_data[] = {0xff, 0xff, 0xff, 0xff, 0xff};
  int64_t size_read_bytes;

  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&stream, &input), NANOARROW_OK);
  EXPECT_EQ(input.data, nullptr);

  EXPECT_EQ(stream.read(&stream, output_data, 2, &size_read_bytes, nullptr),
            NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 2);
  uint8_t output_data1[] = {0x01, 0x02, 0xff, 0xff, 0xff};
  EXPECT_EQ(memcmp(output_data, output_data1, sizeof(output_data)), 0);

  EXPECT_EQ(stream.read(&stream, output_data + 2, 2, &size_read_bytes, nullptr),
            NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 2);
  uint8_t output_data2[] = {0x01, 0x02, 0x03, 0x04, 0xff};
  EXPECT_EQ(memcmp(output_data, output_data2, sizeof(output_data)), 0);

  EXPECT_EQ(stream.read(&stream, output_data + 4, 2, &size_read_bytes, nullptr),
            NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 1);
  uint8_t output_data3[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  EXPECT_EQ(memcmp(output_data, output_data3, sizeof(output_data)), 0);

  EXPECT_EQ(stream.read(&stream, nullptr, 2, &size_read_bytes, nullptr), NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 0);

  EXPECT_EQ(stream.read(&stream, nullptr, 0, &size_read_bytes, nullptr), NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 0);

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, InputStreamFile) {
  uint8_t input_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  FILE* file_ptr = tmpfile();
  ASSERT_NE(file_ptr, nullptr);
  ASSERT_EQ(fwrite(input_data, 1, sizeof(input_data), file_ptr), sizeof(input_data));
  fseek(file_ptr, 0, SEEK_SET);

  struct ArrowIpcInputStream stream;
  uint8_t output_data[] = {0xff, 0xff, 0xff, 0xff, 0xff};
  int64_t size_read_bytes;

  ASSERT_EQ(ArrowIpcInputStreamInitFile(&stream, file_ptr, 1), NANOARROW_OK);

  EXPECT_EQ(stream.read(&stream, output_data, 2, &size_read_bytes, nullptr),
            NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 2);
  uint8_t output_data1[] = {0x01, 0x02, 0xff, 0xff, 0xff};
  EXPECT_EQ(memcmp(output_data, output_data1, sizeof(output_data)), 0);

  EXPECT_EQ(stream.read(&stream, output_data + 2, 2, &size_read_bytes, nullptr),
            NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 2);
  uint8_t output_data2[] = {0x01, 0x02, 0x03, 0x04, 0xff};
  EXPECT_EQ(memcmp(output_data, output_data2, sizeof(output_data)), 0);

  EXPECT_EQ(stream.read(&stream, output_data + 4, 2, &size_read_bytes, nullptr),
            NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 1);
  uint8_t output_data3[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  EXPECT_EQ(memcmp(output_data, output_data3, sizeof(output_data)), 0);

  EXPECT_EQ(stream.read(&stream, nullptr, 2, &size_read_bytes, nullptr), NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 0);

  EXPECT_EQ(stream.read(&stream, nullptr, 0, &size_read_bytes, nullptr), NANOARROW_OK);
  EXPECT_EQ(size_read_bytes, 0);

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, StreamReaderBasic) {
  struct ArrowBuffer input_buffer;
  ArrowBufferInit(&input_buffer);
  ASSERT_EQ(ArrowBufferAppend(&input_buffer, kSimpleSchema, sizeof(kSimpleSchema)),
            NANOARROW_OK);
  ASSERT_EQ(
      ArrowBufferAppend(&input_buffer, kSimpleRecordBatch, sizeof(kSimpleRecordBatch)),
      NANOARROW_OK);

  struct ArrowIpcInputStream input;
  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&input, &input_buffer), NANOARROW_OK);

  struct ArrowArrayStream stream;
  ASSERT_EQ(ArrowIpcArrayStreamReaderInit(&stream, &input, nullptr), NANOARROW_OK);

  struct ArrowSchema schema;

  ASSERT_EQ(stream.get_schema(&stream, &schema), NANOARROW_OK);
  EXPECT_STREQ(schema.format, "+s");
  schema.release(&schema);

  struct ArrowArray array;
  ASSERT_EQ(stream.get_next(&stream, &array), NANOARROW_OK);
  EXPECT_EQ(array.length, 3);
  array.release(&array);

  ASSERT_EQ(stream.get_next(&stream, &array), NANOARROW_OK);
  EXPECT_EQ(array.release, nullptr);

  ASSERT_EQ(stream.get_next(&stream, &array), NANOARROW_OK);
  EXPECT_EQ(array.release, nullptr);

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, StreamReaderBasicNoSharedBuffers) {
  struct ArrowBuffer input_buffer;
  ArrowBufferInit(&input_buffer);
  ASSERT_EQ(ArrowBufferAppend(&input_buffer, kSimpleSchema, sizeof(kSimpleSchema)),
            NANOARROW_OK);
  ASSERT_EQ(
      ArrowBufferAppend(&input_buffer, kSimpleRecordBatch, sizeof(kSimpleRecordBatch)),
      NANOARROW_OK);

  struct ArrowIpcInputStream input;
  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&input, &input_buffer), NANOARROW_OK);

  struct ArrowArrayStream stream;
  struct ArrowIpcArrayStreamReaderOptions options;
  options.field_index = -1;
  options.use_shared_buffers = 0;
  ASSERT_EQ(ArrowIpcArrayStreamReaderInit(&stream, &input, &options), NANOARROW_OK);

  struct ArrowSchema schema;
  ASSERT_EQ(stream.get_schema(&stream, &schema), NANOARROW_OK);
  EXPECT_STREQ(schema.format, "+s");
  schema.release(&schema);

  struct ArrowArray array;
  ASSERT_EQ(stream.get_next(&stream, &array), NANOARROW_OK);
  EXPECT_EQ(array.length, 3);
  array.release(&array);

  ASSERT_EQ(stream.get_next(&stream, &array), NANOARROW_OK);
  EXPECT_EQ(array.release, nullptr);

  ASSERT_EQ(stream.get_next(&stream, &array), NANOARROW_OK);
  EXPECT_EQ(array.release, nullptr);

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, StreamReaderBasicWithEndOfStream) {
  struct ArrowBuffer input_buffer;
  ArrowBufferInit(&input_buffer);
  ASSERT_EQ(ArrowBufferAppend(&input_buffer, kSimpleSchema, sizeof(kSimpleSchema)),
            NANOARROW_OK);
  ASSERT_EQ(
      ArrowBufferAppend(&input_buffer, kSimpleRecordBatch, sizeof(kSimpleRecordBatch)),
      NANOARROW_OK);
  ASSERT_EQ(ArrowBufferAppend(&input_buffer, kEndOfStream, sizeof(kEndOfStream)),
            NANOARROW_OK);

  struct ArrowIpcInputStream input;
  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&input, &input_buffer), NANOARROW_OK);

  struct ArrowArrayStream stream;
  ASSERT_EQ(ArrowIpcArrayStreamReaderInit(&stream, &input, nullptr), NANOARROW_OK);

  struct ArrowSchema schema;
  ASSERT_EQ(stream.get_schema(&stream, &schema), NANOARROW_OK);
  EXPECT_STREQ(schema.format, "+s");
  schema.release(&schema);

  struct ArrowArray array;
  ASSERT_EQ(stream.get_next(&stream, &array), NANOARROW_OK);
  EXPECT_EQ(array.length, 3);
  array.release(&array);

  ASSERT_EQ(stream.get_next(&stream, &array), NANOARROW_OK);
  EXPECT_EQ(array.release, nullptr);

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, StreamReaderExpectedRecordBatch) {
  struct ArrowBuffer input_buffer;
  ArrowBufferInit(&input_buffer);
  ASSERT_EQ(ArrowBufferAppend(&input_buffer, kSimpleSchema, sizeof(kSimpleSchema)),
            NANOARROW_OK);
  ASSERT_EQ(ArrowBufferAppend(&input_buffer, kSimpleSchema, sizeof(kSimpleSchema)),
            NANOARROW_OK);

  struct ArrowIpcInputStream input;
  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&input, &input_buffer), NANOARROW_OK);

  struct ArrowArrayStream stream;
  ASSERT_EQ(ArrowIpcArrayStreamReaderInit(&stream, &input, nullptr), NANOARROW_OK);

  struct ArrowSchema schema;
  ASSERT_EQ(stream.get_schema(&stream, &schema), NANOARROW_OK);
  EXPECT_STREQ(schema.format, "+s");
  schema.release(&schema);

  struct ArrowArray array;
  ASSERT_EQ(stream.get_next(&stream, &array), EINVAL);
  EXPECT_STREQ(stream.get_last_error(&stream),
               "Unexpected message type (expected RecordBatch)");

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, StreamReaderExpectedSchema) {
  struct ArrowBuffer input_buffer;
  ArrowBufferInit(&input_buffer);
  ASSERT_EQ(ArrowBufferAppend(&input_buffer, kSimpleRecordBatch, sizeof(kSimpleSchema)),
            NANOARROW_OK);

  struct ArrowIpcInputStream input;
  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&input, &input_buffer), NANOARROW_OK);

  struct ArrowArrayStream stream;
  ASSERT_EQ(ArrowIpcArrayStreamReaderInit(&stream, &input, nullptr), NANOARROW_OK);

  struct ArrowSchema schema;
  ASSERT_EQ(stream.get_schema(&stream, &schema), EINVAL);
  EXPECT_STREQ(stream.get_last_error(&stream),
               "Unexpected message type at start of input (expected Schema)");

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, StreamReaderUnsupportedFieldIndex) {
  struct ArrowBuffer input_buffer;
  ArrowBufferInit(&input_buffer);
  ASSERT_EQ(ArrowBufferAppend(&input_buffer, kSimpleSchema, sizeof(kSimpleSchema)),
            NANOARROW_OK);
  ASSERT_EQ(
      ArrowBufferAppend(&input_buffer, kSimpleRecordBatch, sizeof(kSimpleRecordBatch)),
      NANOARROW_OK);

  struct ArrowIpcInputStream input;
  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&input, &input_buffer), NANOARROW_OK);

  struct ArrowArrayStream stream;
  struct ArrowIpcArrayStreamReaderOptions options;
  options.field_index = 0;
  options.use_shared_buffers = 0;
  ASSERT_EQ(ArrowIpcArrayStreamReaderInit(&stream, &input, &options), NANOARROW_OK);

  struct ArrowSchema schema;
  ASSERT_EQ(stream.get_schema(&stream, &schema), ENOTSUP);
  EXPECT_STREQ(stream.get_last_error(&stream), "Field index != -1 is not yet supported");

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, StreamReaderEmptyInput) {
  struct ArrowBuffer input_buffer;
  ArrowBufferInit(&input_buffer);

  struct ArrowIpcInputStream input;
  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&input, &input_buffer), NANOARROW_OK);

  struct ArrowArrayStream stream;
  ASSERT_EQ(ArrowIpcArrayStreamReaderInit(&stream, &input, nullptr), NANOARROW_OK);

  struct ArrowSchema schema;
  ASSERT_EQ(stream.get_schema(&stream, &schema), ENODATA);
  EXPECT_STREQ(stream.get_last_error(&stream), "No data available on stream");

  stream.release(&stream);
}

TEST(NanoarrowIpcReader, StreamReaderIncompletePrefix) {
  struct ArrowBuffer input_buffer;
  ArrowBufferInit(&input_buffer);
  ASSERT_EQ(ArrowBufferAppendUInt8(&input_buffer, 0x00), NANOARROW_OK);

  struct ArrowIpcInputStream input;
  ASSERT_EQ(ArrowIpcInputStreamInitBuffer(&input, &input_buffer), NANOARROW_OK);

  struct ArrowArrayStream stream;
  ASSERT_EQ(ArrowIpcArrayStreamReaderInit(&stream, &input, nullptr), NANOARROW_OK);

  struct ArrowSchema schema;
  ASSERT_EQ(stream.get_schema(&stream, &schema), EINVAL);
  EXPECT_STREQ(stream.get_last_error(&stream),
               "Expected at least 8 bytes in remainder of stream");

  stream.release(&stream);
}
