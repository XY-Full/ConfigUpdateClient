// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msg_id.proto

#include "msg_id.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace MSGID {
}  // namespace MSGID
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_msg_5fid_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_msg_5fid_2eproto = nullptr;
const uint32_t TableStruct_msg_5fid_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_msg_5fid_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\014msg_id.proto\022\005MSGID*@\n\005MsgID\022\021\n\rCS_HEA"
  "RT_BEAT\020\000\022\024\n\020CS_CONFIG_UPDATE\020\001\022\016\n\tSC_NO"
  "TIFY\020\220Nb\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_msg_5fid_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_msg_5fid_2eproto = {
    false, false, 95, descriptor_table_protodef_msg_5fid_2eproto,
    "msg_id.proto",
    &descriptor_table_msg_5fid_2eproto_once, nullptr, 0, 0,
    schemas, file_default_instances, TableStruct_msg_5fid_2eproto::offsets,
    nullptr, file_level_enum_descriptors_msg_5fid_2eproto,
    file_level_service_descriptors_msg_5fid_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_msg_5fid_2eproto_getter() {
  return &descriptor_table_msg_5fid_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_msg_5fid_2eproto(&descriptor_table_msg_5fid_2eproto);
namespace MSGID {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MsgID_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_msg_5fid_2eproto);
  return file_level_enum_descriptors_msg_5fid_2eproto[0];
}
bool MsgID_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 10000:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace MSGID
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
