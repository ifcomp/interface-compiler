'use strict';

/**
 * @brief Everbase Technology.
 */
var everbase = everbase || { };

everbase.rpc = everbase.rpc || { }

everbase.rpc.jsonEncoding = everbase.rpc.jsonEncoding || { }

everbase.rpc.jsonEncoding._conversions = everbase.rpc.jsonEncoding._conversions || { }

everbase.rpc.jsonEncoding._handles = everbase.rpc.jsonEncoding._handles || { }

/**
 * @brief Primitive Data Types.
 */
// everbase.primitives = everbase.primitives || { };

/**
 * @brief Everbase Core System.
 */
// everbase.coreSystem = everbase.coreSystem || { };

/**
 * @brief Specifies the behaviour of a device within a domain.
 * 
 * Can be used as flag, but not as a bit mask.
 */
// enum: everbase.coreSystem.deviceRole {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::DeviceRole'] = {
    encode: function(value) { return value }, 
    decode: function(value) { return value } 
}// enum: }

/**
 * @brief Specifies the membership mode of an user or device.
 * 
 * Can be used as a bit mask.
 */
// enum: everbase.coreSystem.membershipMode {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::MembershipMode'] = {
    encode: function(value) { return value }, 
    decode: function(value) { return value } 
}// enum: }

/**
 * @brief Is an device local element for managing the users credentials
 *        for an arbitrary set of domains.
 */
// class: everbase.coreSystem.Profile {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Profile'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.Profile(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief A session defines the current environment, e.g. the currently
 *        active profile. A session is a lightweight object with minimal
 *        initialization cost. Session follows a stack semantic. The most
 *        recent session is the current session for all operations.
 * 
 * Per default a session using the primary profile is on top of the stack.
 */
// class: everbase.coreSystem.Session {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Session'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.Session(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Identifies a user or device.
 * 
 * Supported credential formats are passwords, X.509 certificates, public
 * keys and OAuth authentication token.
 */
// class: everbase.coreSystem.Credential {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Credential'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.Credential(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Enables an organization to manage a set of users, devices,
 *        groups and data.
 */
// class: everbase.coreSystem.Domain {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.Domain(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Is either a user or a user group.
 */
// class: everbase.coreSystem.UserEntity {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::UserEntity'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.UserEntity(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Is a person who uses the EFS.
 */
// class: everbase.coreSystem.User {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::User'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.User(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Specifies a set of users for permission handling and other purposes.
 */
// class: everbase.coreSystem.UserGroup {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::UserGroup'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.UserGroup(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Is either a device or a device group.
 */
// class: everbase.coreSystem.DeviceEntity {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::DeviceEntity'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.DeviceEntity(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Is a system which provides or consumes services within the EFS.
 */
// class: everbase.coreSystem.Device {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Device'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.Device(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Specifies a set of devices for grid management and other purposes.
 */
// class: everbase.coreSystem.DeviceGroup {

everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::DeviceGroup'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.coreSystem.DeviceGroup(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Everbase Object Store.
 */
// everbase.objectStore = everbase.objectStore || { };

/**
 * @brief Defines a set of potentially controllable operations on objects.
 * 
 * Can be used as a bit mask.
 */
// enum: everbase.objectStore.permissionMode {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::PermissionMode'] = {
    encode: function(value) { return value }, 
    decode: function(value) { return value } 
}// enum: }

/**
 * @brief Describes directions of data transfers or idle behaviour.
 * 
 * Can be used as a bit mask.
 */
// enum: everbase.objectStore.syncMode {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncMode'] = {
    encode: function(value) { return value }, 
    decode: function(value) { return value } 
}// enum: }

/**
 * @brief Describes a data transfer process.
 */
// struct: everbase.objectStore.syncStatus {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncStatus'] = {
    encode: function(value) {
        var result = new everbase.objectStore.syncStatus();
        result.mode = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncMode'].encode(value['Mode'])
        result.totalBytesToTransfer = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['TotalBytesToTransfer'])
        result.currentBytesTransferred = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['CurrentBytesTransferred'])
        return result;
    },
    decode: function(value) {
        var result = new everbase.objectStore.syncStatus();
        result.mode = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncMode'].decode(value['Mode'])
        result.totalBytesToTransfer = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['TotalBytesToTransfer'])
        result.currentBytesTransferred = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['CurrentBytesTransferred'])
        return result;
    }
}

// struct: }

/**
 * @brief Describes the cumulated status of multiple data transfer processes.
 */
// struct: everbase.objectStore.cumulatedSyncStatus {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::CumulatedSyncStatus'] = {
    encode: function(value) {
        var result = new everbase.objectStore.cumulatedSyncStatus();
        result.mode = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncMode'].encode(value['Mode'])
        result.totalBytesToDownload = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['TotalBytesToDownload'])
        result.currentBytesDownloaded = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['CurrentBytesDownloaded'])
        result.totalBytesToUpload = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['TotalBytesToUpload'])
        result.currentBytesUploaded = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['CurrentBytesUploaded'])
        return result;
    },
    decode: function(value) {
        var result = new everbase.objectStore.cumulatedSyncStatus();
        result.mode = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncMode'].decode(value['Mode'])
        result.totalBytesToDownload = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['TotalBytesToDownload'])
        result.currentBytesDownloaded = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['CurrentBytesDownloaded'])
        result.totalBytesToUpload = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['TotalBytesToUpload'])
        result.currentBytesUploaded = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['CurrentBytesUploaded'])
        return result;
    }
}

// struct: }

/**
 * @brief Describes a set of timestamps.
 */
// struct: everbase.objectStore.timestamps {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Timestamps'] = {
    encode: function(value) {
        var result = new everbase.objectStore.timestamps();
        result.creation = everbase.rpc.jsonEncoding._conversions['Timestamp'].encode(value['Creation'])
        result.lastModification = everbase.rpc.jsonEncoding._conversions['Timestamp'].encode(value['LastModification'])
        result.lastLocalAccess = everbase.rpc.jsonEncoding._conversions['Timestamp'].encode(value['LastLocalAccess'])
        return result;
    },
    decode: function(value) {
        var result = new everbase.objectStore.timestamps();
        result.creation = everbase.rpc.jsonEncoding._conversions['Timestamp'].decode(value['Creation'])
        result.lastModification = everbase.rpc.jsonEncoding._conversions['Timestamp'].decode(value['LastModification'])
        result.lastLocalAccess = everbase.rpc.jsonEncoding._conversions['Timestamp'].decode(value['LastLocalAccess'])
        return result;
    }
}

// struct: }

/**
 * @brief Describes the various size attributes of a object.
 */
// struct: everbase.objectStore.size {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Size'] = {
    encode: function(value) {
        var result = new everbase.objectStore.size();
        result.contentBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['ContentBytes'])
        result.metaBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['MetaBytes'])
        result.internalBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['InternalBytes'])
        result.totalBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['TotalBytes'])
        return result;
    },
    decode: function(value) {
        var result = new everbase.objectStore.size();
        result.contentBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['ContentBytes'])
        result.metaBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['MetaBytes'])
        result.internalBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['InternalBytes'])
        result.totalBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['TotalBytes'])
        return result;
    }
}

// struct: }

/**
 * @brief Describes where the data of a object is located.
 */
// struct: everbase.objectStore.caching {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Caching'] = {
    encode: function(value) {
        var result = new everbase.objectStore.caching();
        result.uniqueBytesOnGrid = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['UniqueBytesOnGrid'])
        result.uniqueBytesOnPeers = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['UniqueBytesOnPeers'])
        result.uniqueBytesOnLocal = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['UniqueBytesOnLocal'])
        result.totalUniqueBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['TotalUniqueBytes'])
        result.uniqueBytesMissingOnGrid = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['UniqueBytesMissingOnGrid'])
        result.uniqueBytesMissingOnPeer = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['UniqueBytesMissingOnPeer'])
        result.uniqueBytesMissingOnLocal = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['UniqueBytesMissingOnLocal'])
        result.totalUniqueBytesMissing = everbase.rpc.jsonEncoding._conversions['UInt64'].encode(value['TotalUniqueBytesMissing'])
        return result;
    },
    decode: function(value) {
        var result = new everbase.objectStore.caching();
        result.uniqueBytesOnGrid = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['UniqueBytesOnGrid'])
        result.uniqueBytesOnPeers = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['UniqueBytesOnPeers'])
        result.uniqueBytesOnLocal = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['UniqueBytesOnLocal'])
        result.totalUniqueBytes = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['TotalUniqueBytes'])
        result.uniqueBytesMissingOnGrid = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['UniqueBytesMissingOnGrid'])
        result.uniqueBytesMissingOnPeer = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['UniqueBytesMissingOnPeer'])
        result.uniqueBytesMissingOnLocal = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['UniqueBytesMissingOnLocal'])
        result.totalUniqueBytesMissing = everbase.rpc.jsonEncoding._conversions['UInt64'].decode(value['TotalUniqueBytesMissing'])
        return result;
    }
}

// struct: }

/**
 * @brief Describes the status of a object including version information,
 *        timestamps, size, caching, sync status and conflicts.
 */
// struct: everbase.objectStore.status {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Status'] = {
    encode: function(value) {
        var result = new everbase.objectStore.status();
        result.version = everbase.rpc.jsonEncoding._conversions['Uuid'].encode(value['Version'])
        result.author = everbase.rpc.jsonEncoding._conversions['Uuid'].encode(value['Author'])
        result.type = everbase.rpc.jsonEncoding._conversions['Uuid'].encode(value['Type'])
        result.timestamps = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Timestamps'].encode(value['Timestamps'])
        result.size = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Size'].encode(value['Size'])
        result.caching = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Caching'].encode(value['Caching'])
        result.syncStatus = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::CumulatedSyncStatus'].encode(value['SyncStatus'])
        result.conflicts = everbase.rpc.jsonEncoding._conversions['UInt32'].encode(value['Conflicts'])
        result.pinned = everbase.rpc.jsonEncoding._conversions['Boolean'].encode(value['Pinned'])
        result.permission = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::PermissionMode'].encode(value['Permission'])
        return result;
    },
    decode: function(value) {
        var result = new everbase.objectStore.status();
        result.version = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(value['Version'])
        result.author = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(value['Author'])
        result.type = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(value['Type'])
        result.timestamps = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Timestamps'].decode(value['Timestamps'])
        result.size = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Size'].decode(value['Size'])
        result.caching = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Caching'].decode(value['Caching'])
        result.syncStatus = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::CumulatedSyncStatus'].decode(value['SyncStatus'])
        result.conflicts = everbase.rpc.jsonEncoding._conversions['UInt32'].decode(value['Conflicts'])
        result.pinned = everbase.rpc.jsonEncoding._conversions['Boolean'].decode(value['Pinned'])
        result.permission = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::PermissionMode'].decode(value['Permission'])
        return result;
    }
}

// struct: }

/**
 * @brief Describes the status of multiple object including size, caching,
 *        sync status and conflicts.
 */
// struct: everbase.objectStore.cumulatedStatus {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::CumulatedStatus'] = {
    encode: function(value) {
        var result = new everbase.objectStore.cumulatedStatus();
        result.size = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Size'].encode(value['Size'])
        result.caching = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Caching'].encode(value['Caching'])
        result.syncStatus = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::CumulatedSyncStatus'].encode(value['SyncStatus'])
        result.conflicts = everbase.rpc.jsonEncoding._conversions['UInt32'].encode(value['Conflicts'])
        return result;
    },
    decode: function(value) {
        var result = new everbase.objectStore.cumulatedStatus();
        result.size = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Size'].decode(value['Size'])
        result.caching = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Caching'].decode(value['Caching'])
        result.syncStatus = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::CumulatedSyncStatus'].decode(value['SyncStatus'])
        result.conflicts = everbase.rpc.jsonEncoding._conversions['UInt32'].decode(value['Conflicts'])
        return result;
    }
}

// struct: }

/**
 * @brief Specifies the access mode of an user entity. These control the
 *        ability of a user entity to perform operations on the content of
 *        a filesystem node.
 */
// class: everbase.objectStore.Permission {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Permission'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.objectStore.Permission(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Influences the data handling performed by the system.
 * 
 * There are various data handling rules, e.g. geographical restrictions,
 * encryption requirements and more.
 */
// class: everbase.objectStore.Rule {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Rule'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.objectStore.Rule(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Is a set of permissions and rules.
 * 
 * A policy can be either specific to a set of objects (unnamed policy) or
 * a domain (named policy).
 */
// class: everbase.objectStore.Policy {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Policy'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.objectStore.Policy(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief A versioned object which can be used to store arbitrary data.
 */
// class: everbase.objectStore.Object {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Object'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.objectStore.Object(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief An object had been created.
 */
// event: everbase.objectStore.Object.created {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Object::Created'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.Object.created();
        JsEvent.id = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        JsEvent.version = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[1], [  ] );
        JsEvent.type = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[2], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An object had been removed.
 */
// event: everbase.objectStore.Object.erased {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Object::Erased'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.Object.erased();
        JsEvent.id = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        JsEvent.version = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[1], [  ] );
        JsEvent.type = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[2], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief A new version of an object had been created.
 */
// event: everbase.objectStore.Object.newVersion {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Object::NewVersion'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.Object.newVersion();
        JsEvent.id = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        JsEvent.version = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[1], [  ] );
        JsEvent.type = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[2], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief A status update of an object occured.
 */
// event: everbase.objectStore.Object.statusUpdate {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Object::StatusUpdate'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.Object.statusUpdate();
        JsEvent.id = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        JsEvent.status = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Status'].decode(eventValues[1], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief Describes a conflict which occured during a concurrent update of Everbase::ObjectStore::.
 */
// class: everbase.objectStore.Conflict {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Conflict'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.objectStore.Conflict(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief New file version conflicts had been detected.
 */
// event: everbase.objectStore.Conflict.occured {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Conflict::Occured'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.Conflict.occured();
        JsEvent.conflictIds = everbase.rpc.jsonEncoding._conversions['Set'].decode(eventValues[0], [ 'Uuid' ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief File version conflicts had been resolved.
 */
// event: everbase.objectStore.Conflict.resolved {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Conflict::Resolved'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.Conflict.resolved();
        JsEvent.conflictIds = everbase.rpc.jsonEncoding._conversions['Set'].decode(eventValues[0], [ 'Uuid' ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief Describes a sync process of an object.
 */
// class: everbase.objectStore.SyncProcess {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncProcess'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.objectStore.SyncProcess(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief A new sync process started.
 */
// event: everbase.objectStore.SyncProcess.started {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncProcess::Started'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.SyncProcess.started();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An existing sync process completed.
 */
// event: everbase.objectStore.SyncProcess.completed {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncProcess::Completed'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.SyncProcess.completed();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An existing sync process had been paused.
 */
// event: everbase.objectStore.SyncProcess.paused {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncProcess::Paused'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.SyncProcess.paused();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An existing sync process had been resumed.
 */
// event: everbase.objectStore.SyncProcess.resumed {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncProcess::Resumed'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.SyncProcess.resumed();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An existing sync process made progress.
 */
// event: everbase.objectStore.SyncProcess.progress {

everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::SyncProcess::Progress'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.objectStore.SyncProcess.progress();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief Everbase File System.
 */
// everbase.fileSystem = everbase.fileSystem || { };

/**
 * @brief Specifies a unique location of a directory or file within the EFS.
 * 
 * A path is a sequence of names. A name can be any valid unicode string.
 * Each name can be associated with a version identifier.
 */
// class: everbase.fileSystem.Path {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.Path(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Is a key value store.
 * 
 * The key value store can be used by the application developers to save
 * small pieces of data with arbitrary type.
 */
// class: everbase.fileSystem.Metadata {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Metadata'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.Metadata(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Is a filesystem node, which can be one of various things
 *        including a file or directory.
 */
// class: everbase.fileSystem.Node {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Node'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.Node(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief A filesystem node had been created.
 */
// event: everbase.fileSystem.Node.created {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Node::Created'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.Node.created();
        JsEvent.path = everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief A filesystem node had been moved or renamed.
 */
// event: everbase.fileSystem.Node.moved {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Node::Moved'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.Node.moved();
        JsEvent.sourcePath = everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].decode(eventValues[0], [  ] );
        JsEvent.targetPath = everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].decode(eventValues[1], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief A filesystem node had been copied.
 */
// event: everbase.fileSystem.Node.copied {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Node::Copied'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.Node.copied();
        JsEvent.sourcePath = everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].decode(eventValues[0], [  ] );
        JsEvent.targetPath = everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].decode(eventValues[1], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief A filesystem node had been removed.
 */
// event: everbase.fileSystem.Node.removed {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Node::Removed'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.Node.removed();
        JsEvent.path = everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief A new version of a filesystem node had been created.
 */
// event: everbase.fileSystem.Node.newVersion {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Node::NewVersion'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.Node.newVersion();
        JsEvent.versionedPath = everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief A status update of a filesystem node occured.
 */
// event: everbase.fileSystem.Node.statusUpdate {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Node::StatusUpdate'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.Node.statusUpdate();
        JsEvent.path = everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].decode(eventValues[0], [  ] );
        JsEvent.status = everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Status'].decode(eventValues[1], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief Is a file system cataloging structure which contains references
 *        to other files and directories.
 * 
 * Each entry of an directory is associated with an arbitrary name. User
 * space filesystem implementation have to handle the translation of these
 * names to operating system compatible names.
 */
// class: everbase.fileSystem.Directory {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Directory'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.Directory(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Represents the root directory of a profile.
 */
// class: everbase.fileSystem.ProfileRoot {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::ProfileRoot'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.ProfileRoot(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Represents the root directory of a domain.
 */
// class: everbase.fileSystem.DomainRoot {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::DomainRoot'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.DomainRoot(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Is a file system storage structure which can be used to store
 *        arbitrary data.
 */
// class: everbase.fileSystem.File {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::File'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.File(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief Describes a conflict which occured during a concurrent update of files.
 */
// class: everbase.fileSystem.Conflict {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Conflict'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.Conflict(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief New file version conflicts had been detected.
 */
// event: everbase.fileSystem.Conflict.occured {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Conflict::Occured'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.Conflict.occured();
        JsEvent.conflictIds = everbase.rpc.jsonEncoding._conversions['Set'].decode(eventValues[0], [ 'Uuid' ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief File version conflicts had been resolved.
 */
// event: everbase.fileSystem.Conflict.resolved {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Conflict::Resolved'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.Conflict.resolved();
        JsEvent.conflictIds = everbase.rpc.jsonEncoding._conversions['Set'].decode(eventValues[0], [ 'Uuid' ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief Describes a sync process of a file or folder.
 */
// class: everbase.fileSystem.SyncProcess {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::SyncProcess'] = {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) { 
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new everbase.fileSystem.SyncProcess(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
}

// class: }

/**
 * @brief A new sync process started.
 */
// event: everbase.fileSystem.SyncProcess.started {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::SyncProcess::Started'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.SyncProcess.started();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An existing sync process is completed.
 */
// event: everbase.fileSystem.SyncProcess.completed {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::SyncProcess::Completed'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.SyncProcess.completed();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An existing sync process had been paused.
 */
// event: everbase.fileSystem.SyncProcess.paused {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::SyncProcess::Paused'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.SyncProcess.paused();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An existing sync process had been resumed.
 */
// event: everbase.fileSystem.SyncProcess.resumed {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::SyncProcess::Resumed'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.SyncProcess.resumed();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

/**
 * @brief An existing sync process made progress.
 */
// event: everbase.fileSystem.SyncProcess.progress {

everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::SyncProcess::Progress'] = {
    decode: function(eventValues) {
        var JsEvent = new everbase.fileSystem.SyncProcess.progress();
        JsEvent.syncProcessId = everbase.rpc.jsonEncoding._conversions['Uuid'].decode(eventValues[0], [  ] );
        return JsEvent
    }
}
// event: }

