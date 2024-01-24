// NOTE: This entire file should be codegen'ed.

#include "SampleTurboModuleSpec.h"

using namespace rnoh;
using namespace facebook;

static jsi::Value __hostFunction_NativeSampleTurboModuleSpecJSI_voidFunc(jsi::Runtime &rt,
                                                                         react::TurboModule &turboModule,
                                                                         const jsi::Value *args, size_t count) {
    return static_cast<ArkTSTurboModule &>(turboModule).call(rt, "voidFunc", args, count).undefined();
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getNumber(jsi::Runtime &rt,
                                                                           react::TurboModule &turboModule,
                                                                           const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getNumber", args, count)).asNumber();
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getBool(jsi::Runtime &rt,
                                                                           react::TurboModule &turboModule,
                                                                           const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getBool", args, count)).asBool();
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getNull(jsi::Runtime &rt,
                                                                           react::TurboModule &turboModule,
                                                                           const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getNull", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getString(jsi::Runtime &rt,
                                                                             react::TurboModule &turboModule,
                                                                             const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getString", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getObject(jsi::Runtime &rt,
                                                                             react::TurboModule &turboModule,
                                                                             const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getObject", args, count)).asObject(rt);
}

std::string kindToString(const Value& v, Runtime* rt = nullptr) {
  if (v.isUndefined()) {
    return "undefined";
  } else if (v.isNull()) {
    return "null";
  } else if (v.isBool()) {
    return v.getBool() ? "true" : "false";
  } else if (v.isNumber()) {
    return "a number";
  } else if (v.isString()) {
    return "a string";
  } else if (v.isSymbol()) {
    return "a symbol";
  } else if (v.isBigInt()) {
    return "a bigint";
  } else {
    assert(v.isObject() && "Expecting object.");
    return rt != nullptr && v.getObject(*rt).isFunction(*rt) ? "a function"
                                                             : "an object";
  }
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getArray(jsi::Runtime &rt,
                                                                            react::TurboModule &turboModule,
                                                                            const jsi::Value *args, size_t count) {
    if(!args->isObject() || !args->asObject(rt).isArray(rt)) {
        throw JSError(rt, "Value is" + kindToString(*args, &rt) + ", expected an array");
    } else {
        return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getArray", args, count));
    }
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getValue(jsi::Runtime &rt,
                                                                             react::TurboModule &turboModule,
                                                                             const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getValue", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getValueWithPromise(jsi::Runtime &rt,
                                                                             react::TurboModule &turboModule,
                                                                             const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getValueWithPromise", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getValueWithCallback(jsi::Runtime &rt,
                                                                             react::TurboModule &turboModule,
                                                                             const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getValueWithCallback", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getConstants(jsi::Runtime &rt,
                                                                             react::TurboModule &turboModule,
                                                                             const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getConstants", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getRootTag(jsi::Runtime &rt,
                                                                             react::TurboModule &turboModule,
                                                                             const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "getRootTag", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_registerFunction(jsi::Runtime &rt,
                                                                                    react::TurboModule &turboModule,
                                                                                    const jsi::Value *args,
                                                                                    size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "registerFunction", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_doAsyncJob(jsi::Runtime &rt,
                                                                              react::TurboModule &turboModule,
                                                                              const jsi::Value *args, size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).callAsync(rt, "doAsyncJob", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getPromisedArray(jsi::Runtime &rt,
                                                                                    react::TurboModule &turboModule,
                                                                                    const jsi::Value *args,
                                                                                    size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).callAsync(rt, "getPromisedArray", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_displayRNOHError(jsi::Runtime &rt,
                                                                                    react::TurboModule &turboModule,
                                                                                    const jsi::Value *args,
                                                                                    size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "displayRNOHError", args, count));
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_throwExceptionCpp(jsi::Runtime &rt,
                                                                                  react::TurboModule &turboModule,
                                                                                  const jsi::Value *args,
                                                                                  size_t count) {
    throw std::runtime_error("Exception thrown from C++");
}

static jsi::Value __hostFunction_NativeSampleTurboCxxModuleSpecJSI_throwExceptionArk(jsi::Runtime &rt,
                                                                                  react::TurboModule &turboModule,
                                                                                  const jsi::Value *args,
                                                                                  size_t count) {
    return jsi::Value(static_cast<ArkTSTurboModule &>(turboModule).call(rt, "throwExceptionArk", args, count));
}

NativeSampleTurboModuleSpecJSI::NativeSampleTurboModuleSpecJSI(const ArkTSTurboModule::Context ctx,
                                                               const std::string name)
    : ArkTSTurboModule(ctx, name) {
    methodMap_["voidFunc"] = MethodMetadata{0, __hostFunction_NativeSampleTurboModuleSpecJSI_voidFunc};
    methodMap_["getBool"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getBool};
    methodMap_["getNull"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getNull};
    methodMap_["getArray"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getArray};
    methodMap_["getString"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getString};
    methodMap_["getObject"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getObject};
    methodMap_["getNumber"] = MethodMetadata{0, __hostFunction_NativeSampleTurboModuleSpecJSI_getNumber};
    methodMap_["getValue"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getValue};
    methodMap_["getValueWithPromise"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getValueWithPromise};
    methodMap_["getValueWithCallback"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getValueWithCallback};
    methodMap_["getConstants"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getConstants};
    methodMap_["getRootTag"] = MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getRootTag};
    methodMap_["registerFunction"] =
        MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_registerFunction};
    methodMap_["getPromisedArray"] =
        MethodMetadata{0, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_getPromisedArray};
    methodMap_["doAsyncJob"] = MethodMetadata{0, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_doAsyncJob};
    methodMap_["displayRNOHError"] =
        MethodMetadata{1, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_displayRNOHError};
    methodMap_["throwExceptionCpp"] = MethodMetadata{0, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_throwExceptionCpp};
    methodMap_["throwExceptionArk"] = MethodMetadata{0, __hostFunction_NativeSampleTurboCxxModuleSpecJSI_throwExceptionArk};
}