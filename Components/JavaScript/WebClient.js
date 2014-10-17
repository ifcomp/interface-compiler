'use strict';

/**
 * @brief Everbase Technology.
 */
var everbase = everbase || { };

everbase.rpc = everbase.rpc || { }

everbase.rpc.webSocket = everbase.rpc.webSocket || { }

/**
 * @brief Primitive Data Types.
 */
everbase.primitives = everbase.primitives || { };

/**
 * @brief Everbase Core System.
 */
everbase.coreSystem = everbase.coreSystem || { };

/**
 * @brief Specifies the behaviour of a device within a domain.
 * 
 * Can be used as flag, but not as a bit mask.
 */
// enum: everbase.coreSystem.deviceRole {

everbase.coreSystem.deviceRole = { };

/**
 * @brief Grid members are steady members and guarantee availability of
 *        data. The grid is entirely trusted by a domain provider.
 */
everbase.coreSystem.deviceRole.GRID = 0x1;

/**
 * @brief Peers are volatile members.
 */
everbase.coreSystem.deviceRole.PEER = 0x2;

// enum: }

/**
 * @brief Specifies the membership mode of an user or device.
 * 
 * Can be used as a bit mask.
 */
// enum: everbase.coreSystem.membershipMode {

everbase.coreSystem.membershipMode = { };

everbase.coreSystem.membershipMode.JOINREQUEST = 0x1;

everbase.coreSystem.membershipMode.MEMBER = 0x2;

// enum: }

/**
 * @brief Is an device local element for managing the users credentials
 *        for an arbitrary set of domains.
 */
// class: everbase.coreSystem.Profile {

everbase.coreSystem.Profile = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Creates a new profile.
 */
/* Promise [everbase.coreSystem.Profile profile] */ everbase.coreSystem.Profile.create = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Profile::Create',
        uuid(),
        []
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Profile' , [] ];
    });
};

/**
 * @brief Opens an existing profile.
 */
/* Promise [everbase.coreSystem.Profile profile] */ everbase.coreSystem.Profile.open = function(/* Uuid */ profileId) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Profile::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( profileId, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Profile' , [] ];
    });
};

/**
 * @brief Returns the primary profile, which can be used e.g. for desktop applications.
 */
/* Promise [everbase.coreSystem.Profile profile] */ everbase.coreSystem.Profile.getPrimary = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Profile::GetPrimary',
        uuid(),
        []
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Profile' , [] ];
    });
};

/**
 * @brief Returns a set of profile identifiers of the local device.
 */
/* Promise [Set ids] */ everbase.coreSystem.Profile.getAll = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Profile::GetAll',
        uuid(),
        []
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::CoreSystem::Profile'] ];
    });
};

/**
 * @brief Erases the profile.
 * 
 * Once the profile is erased, all subsequent calls to that object fail.
 * In case this is the primary profile, the operation will fail.
 */
/* Promise [] */ everbase.coreSystem.Profile.prototype.erase = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Profile::Erase',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the identifier of the profile.
 */
/* Promise [Uuid id] */ everbase.coreSystem.Profile.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Profile::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

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

everbase.coreSystem.Session = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Starts a new session using the provided profile. The session
 *        will be pushed on top of the session stack.
 */
/* Promise [everbase.coreSystem.Session profile] */ everbase.coreSystem.Session.begin = function(/* everbase.coreSystem.Profile */ profile) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Session::Begin',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Profile'].encode( profile, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Session' , [] ];
    });
};

/**
 * @brief Ends the current session. In case the session is not on the top
 *        of the session stack, all sessions stacked on top of this
 *        session will be closed too.
 * 
 * Once the session is invalidated, all subsequent calls to that object fail.
 */
/* Promise [] */ everbase.coreSystem.Session.prototype.end = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Session::End',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

// class: }

/**
 * @brief Identifies a user or device.
 * 
 * Supported credential formats are passwords, X.509 certificates, public
 * keys and OAuth authentication token.
 */
// class: everbase.coreSystem.Credential {

everbase.coreSystem.Credential = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Returns a username/password credential.
 */
/* Promise [everbase.coreSystem.Credential credential] */ everbase.coreSystem.Credential.fromPassword = function(/* String */ password, /* String */ username) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Credential::FromPassword',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( password, [] ),
            everbase.rpc.jsonEncoding._conversions['String'].encode( username, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Credential' , [] ];
    });
};

/**
 * @brief Returns a X.509 credential.
 */
/* Promise [everbase.coreSystem.Credential credential] */ everbase.coreSystem.Credential.fromX509 = function(/* String */ certificate) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Credential::FromX509',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( certificate, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Credential' , [] ];
    });
};

// class: }

/**
 * @brief Enables an organization to manage a set of users, devices,
 *        groups and data.
 */
// class: everbase.coreSystem.Domain {

everbase.coreSystem.Domain = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Creates a new domain and an initial administrative user object.
 */
/* Promise [everbase.coreSystem.Domain domain] */ everbase.coreSystem.Domain.create = function(/* String */ domainName, /* String */ adminVCard, /* everbase.coreSystem.Credential */ adminCredential) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::Create',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( domainName, [] ),
            everbase.rpc.jsonEncoding._conversions['String'].encode( adminVCard, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Credential'].encode( adminCredential, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Domain' , [] ];
    });
};

/**
 * @brief Opens an existing and authenticated domain.
 */
/* Promise [everbase.coreSystem.Domain domain] */ everbase.coreSystem.Domain.open = function(/* String */ domainName) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( domainName, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Domain' , [] ];
    });
};

/**
 * @brief Authenticate against an existing domain.
 */
/* Promise [everbase.coreSystem.Domain domain] */ everbase.coreSystem.Domain.authenticate = function(/* String */ domainName, /* everbase.coreSystem.Credential */ credential) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::Authenticate',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( domainName, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Credential'].encode( credential, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Domain' , [] ];
    });
};

/**
 * @brief Retrieves all authenticated domains.
 */
/* Promise [Set domain] */ everbase.coreSystem.Domain.getAuthenticated = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::GetAuthenticated',
        uuid(),
        []
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::CoreSystem::Domain'] ];
    });
};

/**
 * @brief Invalidates the authentication.
 * 
 * Once the authentication of the domain is invalidated, all subsequent
 * calls to that object fail.
 */
/* Promise [] */ everbase.coreSystem.Domain.prototype.deauthenticate = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::Deauthenticate',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the domain name.
 */
/* Promise [String domainName] */ everbase.coreSystem.Domain.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Adds an object root identifier to the domain.
 */
/* Promise [] */ everbase.coreSystem.Domain.prototype.addRoot = function(/* Uuid */ type, /* Uuid */ root) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::AddRoot',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( type, [] ),
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( root, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Removes an object root identifier from the domain.
 */
/* Promise [] */ everbase.coreSystem.Domain.prototype.removeRoot = function(/* Uuid */ type) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::RemoveRoot',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( type, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Retrieves an object root identifier of the domain by type.
 */
/* Promise [Uuid root] */ everbase.coreSystem.Domain.prototype.getRoot = function(/* Uuid */ type) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::GetRoot',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( type, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns true if an object root identifier with the provided type
 *        exists on the domain.
 */
/* Promise [Boolean rootStatus] */ everbase.coreSystem.Domain.prototype.hasRoot = function(/* Uuid */ type) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Domain::HasRoot',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( type, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

// class: }

/**
 * @brief Is either a user or a user group.
 */
// class: everbase.coreSystem.UserEntity {

everbase.coreSystem.UserEntity = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Returns the display name of the user entity.
 */
/* Promise [String name] */ everbase.coreSystem.UserEntity.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserEntity::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

// class: }

/**
 * @brief Is a person who uses the EFS.
 */
// class: everbase.coreSystem.User {

everbase.coreSystem.User = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

everbase.coreSystem.User.prototype = Object.create(everbase.coreSystem.UserEntity.prototype);

/**
 * @brief Creates a new user for a specific domain.
 */
/* Promise [everbase.coreSystem.User user] */ everbase.coreSystem.User.createJoinRequest = function(/* String */ vCard, /* everbase.coreSystem.Credential */ credential, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::CreateJoinRequest',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( vCard, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Credential'].encode( credential, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::User' , [] ];
    });
};

/**
 * @brief Returns all users of a domain.
 */
/* Promise [Set user] */ everbase.coreSystem.User.getAll = function(/* everbase.coreSystem.Domain */ domain, /* everbase.coreSystem.membershipMode */ membershipMode) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::GetAll',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::MembershipMode'].encode( membershipMode, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::CoreSystem::User'] ];
    });
};

/**
 * @brief Returns the authenticated user of a domain.
 */
/* Promise [everbase.coreSystem.User user] */ everbase.coreSystem.User.getAuthenticated = function(/* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::GetAuthenticated',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::User' , [] ];
    });
};

/**
 * @brief Opens an existing user by id.
 */
/* Promise [everbase.coreSystem.User user] */ everbase.coreSystem.User.open = function(/* Uuid */ userId, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( userId, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::User' , [] ];
    });
};

/**
 * @brief Returns the identifier of the user.
 */
/* Promise [Uuid id] */ everbase.coreSystem.User.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * 
 * Once the user is erased, all subsequent calls to that object fail.
 * @brief Erases the user.
 */
/* Promise [] */ everbase.coreSystem.User.prototype.erase = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::Erase',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the display name of the user.
 */
/* Promise [String name] */ everbase.coreSystem.User.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Sets the vcard of the user.
 */
/* Promise [] */ everbase.coreSystem.User.prototype.setVCard = function(/* String */ vCard) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::SetVCard',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( vCard, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the vcard of the user.
 */
/* Promise [String vCard] */ everbase.coreSystem.User.prototype.getVCard = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::GetVCard',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Sets the credential of the user.
 */
/* Promise [] */ everbase.coreSystem.User.prototype.setCredential = function(/* everbase.coreSystem.Credential */ credential, /* everbase.coreSystem.Credential */ oldCredential) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::SetCredential',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Credential'].encode( credential, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Credential'].encode( oldCredential, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the credential of the user.
 */
/* Promise [everbase.coreSystem.Credential credential] */ everbase.coreSystem.User.prototype.getCredential = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::GetCredential',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Credential' , [] ];
    });
};

/**
 * @brief Returns the current membership mode.
 */
/* Promise [everbase.coreSystem.membershipMode membershipMode] */ everbase.coreSystem.User.prototype.getMembershipMode = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::GetMembershipMode',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::MembershipMode' , [] ];
    });
};

/**
 * @brief Performs a join.
 */
/* Promise [] */ everbase.coreSystem.User.prototype.join = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::Join',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the fingerprint.
 */
/* Promise [String fingerprint] */ everbase.coreSystem.User.prototype.getFingerprint = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::User::GetFingerprint',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

// class: }

/**
 * @brief Specifies a set of users for permission handling and other purposes.
 */
// class: everbase.coreSystem.UserGroup {

everbase.coreSystem.UserGroup = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

everbase.coreSystem.UserGroup.prototype = Object.create(everbase.coreSystem.UserEntity.prototype);

/**
 * @brief Creates a new user group for a specific domain.
 */
/* Promise [everbase.coreSystem.UserGroup group] */ everbase.coreSystem.UserGroup.create = function(/* String */ name, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::Create',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::UserGroup' , [] ];
    });
};

/**
 * @brief Returns all user group of a specific domain.
 */
/* Promise [Set groups] */ everbase.coreSystem.UserGroup.getAll = function(/* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::GetAll',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::CoreSystem::UserGroup'] ];
    });
};

/**
 * @brief Opens an existing usergroup by id.
 */
/* Promise [everbase.coreSystem.UserGroup userGroup] */ everbase.coreSystem.UserGroup.open = function(/* Uuid */ userGroupId, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( userGroupId, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::UserGroup' , [] ];
    });
};

/**
 * @brief Returns the identifier of the user group.
 */
/* Promise [Uuid id] */ everbase.coreSystem.UserGroup.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Erases the user group.
 * 
 * Once the user group is erased, all subsequent calls to that object fail.
 */
/* Promise [] */ everbase.coreSystem.UserGroup.prototype.erase = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::Erase',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Sets the display name of the user group.
 */
/* Promise [] */ everbase.coreSystem.UserGroup.prototype.setName = function(/* String */ name) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::SetName',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the display name of the user group.
 */
/* Promise [String name] */ everbase.coreSystem.UserGroup.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Adds a user to the user group.
 */
/* Promise [] */ everbase.coreSystem.UserGroup.prototype.addUser = function(/* everbase.coreSystem.User */ user) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::AddUser',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::User'].encode( user, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Removes a user from the user group.
 */
/* Promise [] */ everbase.coreSystem.UserGroup.prototype.removeUser = function(/* everbase.coreSystem.User */ user) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::RemoveUser',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::User'].encode( user, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns all users of the user group.
 */
/* Promise [Set users] */ everbase.coreSystem.UserGroup.prototype.getUsers = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::UserGroup::GetUsers',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::CoreSystem::User'] ];
    });
};

// class: }

/**
 * @brief Is either a device or a device group.
 */
// class: everbase.coreSystem.DeviceEntity {

everbase.coreSystem.DeviceEntity = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Returns the display name of the device entity.
 */
/* Promise [String name] */ everbase.coreSystem.DeviceEntity.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceEntity::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

// class: }

/**
 * @brief Is a system which provides or consumes services within the EFS.
 */
// class: everbase.coreSystem.Device {

everbase.coreSystem.Device = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

everbase.coreSystem.Device.prototype = Object.create(everbase.coreSystem.DeviceEntity.prototype);

/**
 * @brief Returns all device Everbase::CoreSystem of a domain.
 */
/* Promise [Set device] */ everbase.coreSystem.Device.getAll = function(/* everbase.coreSystem.Domain */ domain, /* everbase.coreSystem.membershipMode */ membershipMode) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::GetAll',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::MembershipMode'].encode( membershipMode, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::CoreSystem::Device'] ];
    });
};

/**
 * @brief Opens an existing device by id.
 */
/* Promise [everbase.coreSystem.Device device] */ everbase.coreSystem.Device.open = function(/* Uuid */ deviceId, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( deviceId, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Device' , [] ];
    });
};

/**
 * @brief Returns the identifier of the device.
 */
/* Promise [Uuid id] */ everbase.coreSystem.Device.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Erases the device object.
 * 
 * Once the device object is erased, all subsequent calls to that object fail.
 */
/* Promise [] */ everbase.coreSystem.Device.prototype.erase = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::Erase',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Sets the display name of the device object.
 */
/* Promise [] */ everbase.coreSystem.Device.prototype.setName = function(/* String */ name) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::SetName',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the display name of the device object.
 */
/* Promise [String name] */ everbase.coreSystem.Device.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Sets the role of the device object.
 */
/* Promise [] */ everbase.coreSystem.Device.prototype.setRole = function(/* everbase.coreSystem.deviceRole */ role) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::SetRole',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::DeviceRole'].encode( role, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the role of the device object.
 */
/* Promise [everbase.coreSystem.deviceRole role] */ everbase.coreSystem.Device.prototype.getRole = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::GetRole',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::DeviceRole' , [] ];
    });
};

/**
 * @brief Returns the current membership mode.
 */
/* Promise [everbase.coreSystem.membershipMode membershipMode] */ everbase.coreSystem.Device.prototype.getMembershipMode = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::GetMembershipMode',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::MembershipMode' , [] ];
    });
};

/**
 * @brief Performs a join.
 */
/* Promise [] */ everbase.coreSystem.Device.prototype.join = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::Join',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the fingerprint.
 */
/* Promise [String fingerprint] */ everbase.coreSystem.Device.prototype.getFingerprint = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::Device::GetFingerprint',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

// class: }

/**
 * @brief Specifies a set of devices for grid management and other purposes.
 */
// class: everbase.coreSystem.DeviceGroup {

everbase.coreSystem.DeviceGroup = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

everbase.coreSystem.DeviceGroup.prototype = Object.create(everbase.coreSystem.DeviceEntity.prototype);

/**
 * @brief Creates a device group for a domain.
 */
/* Promise [everbase.coreSystem.DeviceGroup group] */ everbase.coreSystem.DeviceGroup.create = function(/* String */ name, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::Create',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::DeviceGroup' , [] ];
    });
};

/**
 * @brief Returns all device groups of a domain.
 */
/* Promise [Set groups] */ everbase.coreSystem.DeviceGroup.getAll = function(/* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::GetAll',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::CoreSystem::DeviceGroup'] ];
    });
};

/**
 * @brief Opens an existing devicegroup by id.
 */
/* Promise [everbase.coreSystem.DeviceGroup deviceGroup] */ everbase.coreSystem.DeviceGroup.open = function(/* Uuid */ deviceGroupId, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( deviceGroupId, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::DeviceGroup' , [] ];
    });
};

/**
 * @brief Returns the identifier of the device group.
 */
/* Promise [Uuid id] */ everbase.coreSystem.DeviceGroup.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Erases the device group.
 * 
 * Once the device group is erased, all subsequent calls to that object fail.
 */
/* Promise [] */ everbase.coreSystem.DeviceGroup.prototype.erase = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::Erase',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Sets the display name of the device group.
 */
/* Promise [] */ everbase.coreSystem.DeviceGroup.prototype.setName = function(/* String */ name) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::SetName',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the display name of the device group.
 */
/* Promise [String name] */ everbase.coreSystem.DeviceGroup.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Adds a device object to the device group.
 */
/* Promise [] */ everbase.coreSystem.DeviceGroup.prototype.addDevice = function(/* everbase.coreSystem.Device */ device) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::AddDevice',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Device'].encode( device, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Removes a device object of the device group.
 */
/* Promise [] */ everbase.coreSystem.DeviceGroup.prototype.removeDevice = function(/* everbase.coreSystem.Device */ device) {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::RemoveDevice',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Device'].encode( device, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns all device Everbase::CoreSystem of the device group.
 */
/* Promise [Set devices] */ everbase.coreSystem.DeviceGroup.prototype.getDevices = function() {
    var message = 
    [
        'call',
        'Everbase::CoreSystem::DeviceGroup::GetDevices',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::CoreSystem::Device'] ];
    });
};

// class: }

/**
 * @brief Everbase Object Store.
 */
everbase.objectStore = everbase.objectStore || { };

/**
 * @brief Defines a set of potentially controllable operations on objects.
 * 
 * Can be used as a bit mask.
 */
// enum: everbase.objectStore.permissionMode {

everbase.objectStore.permissionMode = { };

/**
 * @brief Read operation.
 */
everbase.objectStore.permissionMode.READ = 0x1;

/**
 * @brief Write operation.
 */
everbase.objectStore.permissionMode.WRITE = 0x2;

/**
 * @brief Manage operation, includes management of permissions and rules.
 */
everbase.objectStore.permissionMode.MANAGE = 0x4;

// enum: }

/**
 * @brief Describes directions of data transfers or idle behaviour.
 * 
 * Can be used as a bit mask.
 */
// enum: everbase.objectStore.syncMode {

everbase.objectStore.syncMode = { };

/**
 * @brief No current or scheduled data transfer.
 */
everbase.objectStore.syncMode.IDLE = 0x1;

/**
 * @brief Data transfer scheduled.
 */
everbase.objectStore.syncMode.WAITING = 0x2;

/**
 * @brief Current data transfer paused.
 */
everbase.objectStore.syncMode.PAUSED = 0x4;

/**
 * @brief Current data transfer is an upload.
 */
everbase.objectStore.syncMode.UPLOADING = 0x8;

/**
 * @brief Current data transfer is a download.
 */
everbase.objectStore.syncMode.DOWNLOADING = 0x10;

// enum: }

/**
 * @brief Describes a data transfer process.
 */
// struct: everbase.objectStore.syncStatus {

everbase.objectStore.syncStatus = function() { };

/**
 * @brief Current direction of data transfer.
 */
Object.defineProperty(everbase.objectStore.syncStatus.prototype, 'mode', { get: function() { return this._mode }, set: function(mode) { this._mode = mode } } ); /* everbase.objectStore.syncMode */

/**
 * @brief Number of total bytes to transfer.
 */
Object.defineProperty(everbase.objectStore.syncStatus.prototype, 'totalBytesToTransfer', { get: function() { return this._totalBytesToTransfer }, set: function(totalBytesToTransfer) { this._totalBytesToTransfer = totalBytesToTransfer } } ); /* UInt64 */

/**
 * @brief Number of current bytes transferred.
 */
Object.defineProperty(everbase.objectStore.syncStatus.prototype, 'currentBytesTransferred', { get: function() { return this._currentBytesTransferred }, set: function(currentBytesTransferred) { this._currentBytesTransferred = currentBytesTransferred } } ); /* UInt64 */

// struct: }

/**
 * @brief Describes the cumulated status of multiple data transfer processes.
 */
// struct: everbase.objectStore.cumulatedSyncStatus {

everbase.objectStore.cumulatedSyncStatus = function() { };

/**
 * @brief Current directions of data transfer.
 */
Object.defineProperty(everbase.objectStore.cumulatedSyncStatus.prototype, 'mode', { get: function() { return this._mode }, set: function(mode) { this._mode = mode } } ); /* everbase.objectStore.syncMode */

/**
 * @brief Number of total bytes to download.
 */
Object.defineProperty(everbase.objectStore.cumulatedSyncStatus.prototype, 'totalBytesToDownload', { get: function() { return this._totalBytesToDownload }, set: function(totalBytesToDownload) { this._totalBytesToDownload = totalBytesToDownload } } ); /* UInt64 */

/**
 * @brief Number of bytes downloaded.
 */
Object.defineProperty(everbase.objectStore.cumulatedSyncStatus.prototype, 'currentBytesDownloaded', { get: function() { return this._currentBytesDownloaded }, set: function(currentBytesDownloaded) { this._currentBytesDownloaded = currentBytesDownloaded } } ); /* UInt64 */

/**
 * @brief Number of total bytes to upload.
 */
Object.defineProperty(everbase.objectStore.cumulatedSyncStatus.prototype, 'totalBytesToUpload', { get: function() { return this._totalBytesToUpload }, set: function(totalBytesToUpload) { this._totalBytesToUpload = totalBytesToUpload } } ); /* UInt64 */

/**
 * @brief Number of bytes uploaded.
 */
Object.defineProperty(everbase.objectStore.cumulatedSyncStatus.prototype, 'currentBytesUploaded', { get: function() { return this._currentBytesUploaded }, set: function(currentBytesUploaded) { this._currentBytesUploaded = currentBytesUploaded } } ); /* UInt64 */

// struct: }

/**
 * @brief Describes a set of timestamps.
 */
// struct: everbase.objectStore.timestamps {

everbase.objectStore.timestamps = function() { };

/**
 * @brief Time of object creation.
 */
Object.defineProperty(everbase.objectStore.timestamps.prototype, 'creation', { get: function() { return this._creation }, set: function(creation) { this._creation = creation } } ); /* Timestamp */

/**
 * @brief Time of last modification.
 */
Object.defineProperty(everbase.objectStore.timestamps.prototype, 'lastModification', { get: function() { return this._lastModification }, set: function(lastModification) { this._lastModification = lastModification } } ); /* Timestamp */

/**
 * @brief Time of last access on local device.
 */
Object.defineProperty(everbase.objectStore.timestamps.prototype, 'lastLocalAccess', { get: function() { return this._lastLocalAccess }, set: function(lastLocalAccess) { this._lastLocalAccess = lastLocalAccess } } ); /* Timestamp */

// struct: }

/**
 * @brief Describes the various size attributes of a object.
 */
// struct: everbase.objectStore.size {

everbase.objectStore.size = function() { };

/**
 * @brief Number of bytes used for content data.
 */
Object.defineProperty(everbase.objectStore.size.prototype, 'contentBytes', { get: function() { return this._contentBytes }, set: function(contentBytes) { this._contentBytes = contentBytes } } ); /* UInt64 */

/**
 * @brief Number of bytes used for meta data.
 */
Object.defineProperty(everbase.objectStore.size.prototype, 'metaBytes', { get: function() { return this._metaBytes }, set: function(metaBytes) { this._metaBytes = metaBytes } } ); /* UInt64 */

/**
 * @brief Number of bytes used for internal data.
 */
Object.defineProperty(everbase.objectStore.size.prototype, 'internalBytes', { get: function() { return this._internalBytes }, set: function(internalBytes) { this._internalBytes = internalBytes } } ); /* UInt64 */

/**
 * @brief Number of total bytes including internal data, meta data and
 *        content data.
 */
Object.defineProperty(everbase.objectStore.size.prototype, 'totalBytes', { get: function() { return this._totalBytes }, set: function(totalBytes) { this._totalBytes = totalBytes } } ); /* UInt64 */

// struct: }

/**
 * @brief Describes where the data of a object is located.
 */
// struct: everbase.objectStore.caching {

everbase.objectStore.caching = function() { };

/**
 * @brief Number of unique bytes cached on the grid.
 */
Object.defineProperty(everbase.objectStore.caching.prototype, 'uniqueBytesOnGrid', { get: function() { return this._uniqueBytesOnGrid }, set: function(uniqueBytesOnGrid) { this._uniqueBytesOnGrid = uniqueBytesOnGrid } } ); /* UInt64 */

/**
 * @brief Number of unique bytes cached on the peer overlay network.
 */
Object.defineProperty(everbase.objectStore.caching.prototype, 'uniqueBytesOnPeers', { get: function() { return this._uniqueBytesOnPeers }, set: function(uniqueBytesOnPeers) { this._uniqueBytesOnPeers = uniqueBytesOnPeers } } ); /* UInt64 */

/**
 * @brief Number of unique bytes cached on the local device.
 */
Object.defineProperty(everbase.objectStore.caching.prototype, 'uniqueBytesOnLocal', { get: function() { return this._uniqueBytesOnLocal }, set: function(uniqueBytesOnLocal) { this._uniqueBytesOnLocal = uniqueBytesOnLocal } } ); /* UInt64 */

/**
 * @brief Number of unique bytes cached on the entire distributed system.
 */
Object.defineProperty(everbase.objectStore.caching.prototype, 'totalUniqueBytes', { get: function() { return this._totalUniqueBytes }, set: function(totalUniqueBytes) { this._totalUniqueBytes = totalUniqueBytes } } ); /* UInt64 */

/**
 * @brief Number of unique bytes missing on the grid.
 */
Object.defineProperty(everbase.objectStore.caching.prototype, 'uniqueBytesMissingOnGrid', { get: function() { return this._uniqueBytesMissingOnGrid }, set: function(uniqueBytesMissingOnGrid) { this._uniqueBytesMissingOnGrid = uniqueBytesMissingOnGrid } } ); /* UInt64 */

/**
 * @brief Number of unique bytes missing on the peer.
 */
Object.defineProperty(everbase.objectStore.caching.prototype, 'uniqueBytesMissingOnPeer', { get: function() { return this._uniqueBytesMissingOnPeer }, set: function(uniqueBytesMissingOnPeer) { this._uniqueBytesMissingOnPeer = uniqueBytesMissingOnPeer } } ); /* UInt64 */

/**
 * @brief Number of unique bytes missing on the local device.
 */
Object.defineProperty(everbase.objectStore.caching.prototype, 'uniqueBytesMissingOnLocal', { get: function() { return this._uniqueBytesMissingOnLocal }, set: function(uniqueBytesMissingOnLocal) { this._uniqueBytesMissingOnLocal = uniqueBytesMissingOnLocal } } ); /* UInt64 */

/**
 * @brief Number of unique bytes missing on the entire distributed system.
 */
Object.defineProperty(everbase.objectStore.caching.prototype, 'totalUniqueBytesMissing', { get: function() { return this._totalUniqueBytesMissing }, set: function(totalUniqueBytesMissing) { this._totalUniqueBytesMissing = totalUniqueBytesMissing } } ); /* UInt64 */

// struct: }

/**
 * @brief Describes the status of a object including version information,
 *        timestamps, size, caching, sync status and conflicts.
 */
// struct: everbase.objectStore.status {

everbase.objectStore.status = function() { };

/**
 * @brief Version of object reflected within the status information.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'version', { get: function() { return this._version }, set: function(version) { this._version = version } } ); /* Uuid */

/**
 * @brief User id of the author of this version.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'author', { get: function() { return this._author }, set: function(author) { this._author = author } } ); /* Uuid */

/**
 * @brief Type of object.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'type', { get: function() { return this._type }, set: function(type) { this._type = type } } ); /* Uuid */

/**
 * @brief Various timestamps of object.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'timestamps', { get: function() { return this._timestamps }, set: function(timestamps) { this._timestamps = timestamps } } ); /* everbase.objectStore.timestamps */

/**
 * @brief Various size attributes of object.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'size', { get: function() { return this._size }, set: function(size) { this._size = size } } ); /* everbase.objectStore.size */

/**
 * @brief Caching status of object.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'caching', { get: function() { return this._caching }, set: function(caching) { this._caching = caching } } ); /* everbase.objectStore.caching */

/**
 * @brief Sync status of object.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'syncStatus', { get: function() { return this._syncStatus }, set: function(syncStatus) { this._syncStatus = syncStatus } } ); /* everbase.objectStore.cumulatedSyncStatus */

/**
 * @brief Number of pair-wise conflicts within object.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'conflicts', { get: function() { return this._conflicts }, set: function(conflicts) { this._conflicts = conflicts } } ); /* UInt32 */

/**
 * @brief Pinning status.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'pinned', { get: function() { return this._pinned }, set: function(pinned) { this._pinned = pinned } } ); /* Boolean */

/**
 * @brief Effective permission mode of the authenticated user.
 */
Object.defineProperty(everbase.objectStore.status.prototype, 'permission', { get: function() { return this._permission }, set: function(permission) { this._permission = permission } } ); /* everbase.objectStore.permissionMode */

// struct: }

/**
 * @brief Describes the status of multiple object including size, caching,
 *        sync status and conflicts.
 */
// struct: everbase.objectStore.cumulatedStatus {

everbase.objectStore.cumulatedStatus = function() { };

/**
 * @brief Various size attributes of object.
 */
Object.defineProperty(everbase.objectStore.cumulatedStatus.prototype, 'size', { get: function() { return this._size }, set: function(size) { this._size = size } } ); /* everbase.objectStore.size */

/**
 * @brief Caching status of object.
 */
Object.defineProperty(everbase.objectStore.cumulatedStatus.prototype, 'caching', { get: function() { return this._caching }, set: function(caching) { this._caching = caching } } ); /* everbase.objectStore.caching */

/**
 * @brief Sync status of object.
 */
Object.defineProperty(everbase.objectStore.cumulatedStatus.prototype, 'syncStatus', { get: function() { return this._syncStatus }, set: function(syncStatus) { this._syncStatus = syncStatus } } ); /* everbase.objectStore.cumulatedSyncStatus */

/**
 * @brief Number of pair-wise conflicts within object.
 */
Object.defineProperty(everbase.objectStore.cumulatedStatus.prototype, 'conflicts', { get: function() { return this._conflicts }, set: function(conflicts) { this._conflicts = conflicts } } ); /* UInt32 */

// struct: }

/**
 * @brief Specifies the access mode of an user entity. These control the
 *        ability of a user entity to perform operations on the content of
 *        a filesystem node.
 */
// class: everbase.objectStore.Permission {

everbase.objectStore.Permission = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Sets the user entity which is subject of this permission object.
 */
/* Promise [] */ everbase.objectStore.Permission.prototype.setEntity = function(/* everbase.coreSystem.UserEntity */ entity) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Permission::SetEntity',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::UserEntity'].encode( entity, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the user entity which is subject of this permission object.
 */
/* Promise [everbase.coreSystem.UserEntity entity] */ everbase.objectStore.Permission.prototype.getEntity = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Permission::GetEntity',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::UserEntity' , [] ];
    });
};

/**
 * @brief Sets the permission mode.
 */
/* Promise [] */ everbase.objectStore.Permission.prototype.setMode = function(/* everbase.objectStore.permissionMode */ mode) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Permission::SetMode',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::PermissionMode'].encode( mode, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the permission mode.
 */
/* Promise [everbase.objectStore.permissionMode mode] */ everbase.objectStore.Permission.prototype.getMode = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Permission::GetMode',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::PermissionMode' , [] ];
    });
};

// class: }

/**
 * @brief Influences the data handling performed by the system.
 * 
 * There are various data handling rules, e.g. geographical restrictions,
 * encryption requirements and more.
 */
// class: everbase.objectStore.Rule {

everbase.objectStore.Rule = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

// class: }

/**
 * @brief Is a set of permissions and rules.
 * 
 * A policy can be either specific to a set of objects (unnamed policy) or
 * a domain (named policy).
 */
// class: everbase.objectStore.Policy {

everbase.objectStore.Policy = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Creates a new named policy for a domain.
 */
/* Promise [everbase.objectStore.Policy policy] */ everbase.objectStore.Policy.create = function(/* String */ name, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::Create',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Policy' , [] ];
    });
};

/**
 * @brief Returns all policies of a domain.
 */
/* Promise [Set policies] */ everbase.objectStore.Policy.getAll = function(/* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::GetAll',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::ObjectStore::Policy'] ];
    });
};

/**
 * @brief Erases the policy.
 * 
 * Once the policy is erased, all subsequent calls to that object fail.
 */
/* Promise [] */ everbase.objectStore.Policy.prototype.erase = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::Erase',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Sets the name of the policy.
 */
/* Promise [] */ everbase.objectStore.Policy.prototype.setName = function(/* String */ name) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::SetName',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the name of the policy.
 */
/* Promise [String name] */ everbase.objectStore.Policy.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Adds a new permission object to the policy.
 */
/* Promise [everbase.objectStore.Permission permission] */ everbase.objectStore.Policy.prototype.addPermission = function(/* everbase.coreSystem.UserEntity */ entity, /* everbase.objectStore.permissionMode */ mode) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::AddPermission',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::UserEntity'].encode( entity, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::PermissionMode'].encode( mode, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Permission' , [] ];
    });
};

/**
 * @brief Removes a permission object from the policy.
 */
/* Promise [] */ everbase.objectStore.Policy.prototype.removePermission = function(/* everbase.objectStore.Permission */ permission) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::RemovePermission',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Permission'].encode( permission, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns all permission Everbase::ObjectStore:: of the policy.
 */
/* Promise [Set permissions] */ everbase.objectStore.Policy.prototype.getPermissions = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::GetPermissions',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::ObjectStore::Permission'] ];
    });
};

/**
 * @brief Adds a rule object to the policy.
 */
/* Promise [] */ everbase.objectStore.Policy.prototype.addRule = function(/* everbase.objectStore.Rule */ rule) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::AddRule',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Rule'].encode( rule, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Removes a rule object from the policy.
 */
/* Promise [] */ everbase.objectStore.Policy.prototype.removeRule = function(/* everbase.objectStore.Rule */ rule) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::RemoveRule',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Rule'].encode( rule, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns all rule Everbase::ObjectStore:: of the policy.
 */
/* Promise [Set rules] */ everbase.objectStore.Policy.prototype.getRules = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Policy::GetRules',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::ObjectStore::Rule'] ];
    });
};

// class: }

/**
 * @brief A versioned object which can be used to store arbitrary data.
 */
// class: everbase.objectStore.Object {

everbase.objectStore.Object = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Creates a new object.
 * 
 * Requires a commit to persist that object.
 */
/* Promise [everbase.objectStore.Object object] */ everbase.objectStore.Object.create = function(/* Uuid */ type, /* everbase.coreSystem.Domain */ domain) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Create',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( type, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::CoreSystem::Domain'].encode( domain, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Object' , [] ];
    });
};

/**
 * @brief Clones a specific version of an existing object.
 * 
 * Requires a commit to persist that object.
 */
/* Promise [everbase.objectStore.Object object] */ everbase.objectStore.Object.clone = function(/* Uuid */ id, /* Uuid */ version, /* Boolean */ copyHistory) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Clone',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( version, [] ),
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( copyHistory, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Object' , [] ];
    });
};

/**
 * 
 * Throws exception in case there are multiple head versions.
 * @brief Opens head version of an existing object.
 */
/* Promise [everbase.objectStore.Object object] */ everbase.objectStore.Object.openHead = function(/* Uuid */ id) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::OpenHead',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Object' , [] ];
    });
};

/**
 * @brief Opens head versions of an existing object.
 */
/* Promise [Set object] */ everbase.objectStore.Object.openHeads = function(/* Uuid */ id) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::OpenHeads',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::ObjectStore::Object'] ];
    });
};

/**
 * @brief Opens a specific version of an existing object.
 */
/* Promise [everbase.objectStore.Object object] */ everbase.objectStore.Object.openVersion = function(/* Uuid */ id, /* Uuid */ version) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::OpenVersion',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( version, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Object' , [] ];
    });
};

/**
 * @brief Sets the pinning status of the object.
 */
/* Promise [] */ everbase.objectStore.Object.setPinning = function(/* Uuid */ id, /* Boolean */ pinning) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::SetPinning',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( pinning, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Gets the pinning status of the object.
 */
/* Promise [Boolean pinning] */ everbase.objectStore.Object.getPinning = function(/* Uuid */ id) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetPinning',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Returns status information of an object.
 */
/* Promise [everbase.objectStore.status status] */ everbase.objectStore.Object.getStatus = function(/* Uuid */ id) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetStatus',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Status' , [] ];
    });
};

/**
 * @brief Returns status information of an object.
 */
/* Promise [everbase.objectStore.status status] */ everbase.objectStore.Object.getVersionStatus = function(/* Uuid */ id, /* Uuid */ version) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetVersionStatus',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( version, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Status' , [] ];
    });
};

/**
 * @brief Returns object id.
 */
/* Promise [Uuid id] */ everbase.objectStore.Object.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns current version id.
 */
/* Promise [Uuid version] */ everbase.objectStore.Object.prototype.getVersion = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetVersion',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns version ids of predecessors.
 */
/* Promise [Set version] */ everbase.objectStore.Object.prototype.getParentVersions = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetParentVersions',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Uuid'] ];
    });
};

/**
 * @brief Returns type of the object.
 */
/* Promise [Uuid type] */ everbase.objectStore.Object.prototype.getType = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetType',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Updates the type of the object.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.setType = function(/* Uuid */ type) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::SetType',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( type, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Attaches a policy to the object.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.attachPolicy = function(/* everbase.objectStore.Policy */ policy, /* everbase.objectStore.Policy */ after) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::AttachPolicy',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Policy'].encode( policy, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Policy'].encode( after, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Detaches a policy from the object.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.detachPolicy = function(/* everbase.objectStore.Policy */ policy) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::DetachPolicy',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Policy'].encode( policy, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns all attached policies of the object.
 */
/* Promise [Set policies] */ everbase.objectStore.Object.prototype.getPolicies = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetPolicies',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::ObjectStore::Policy'] ];
    });
};

/**
 * @brief Returns the effective policy of the object.
 * 
 * The effective policy is a computed object derived from all applicable
 * attached and inherited policies.
 */
/* Promise [everbase.objectStore.Policy policy] */ everbase.objectStore.Object.prototype.getEffectivePolicy = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetEffectivePolicy',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Policy' , [] ];
    });
};

/**
 * @brief Returns true if policy inheritance is activated (default).
 */
/* Promise [Boolean inheritance] */ everbase.objectStore.Object.prototype.getPolicyInheritance = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetPolicyInheritance',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Enables or disables policy inheritance.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.setPolicyInheritance = function(/* Boolean */ inheritance) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::SetPolicyInheritance',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( inheritance, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the current parent object.
 */
/* Promise [Uuid parentObjectId] */ everbase.objectStore.Object.prototype.getParentObject = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetParentObject',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Sets the parent object.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.setParentObject = function(/* Uuid */ parentObjectId) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::SetParentObject',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( parentObjectId, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns true if a parent object is available.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.hasParentObject = function(/* Boolean */ parentObjectStatus) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::HasParentObject',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( parentObjectStatus, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Writes a block of bytes to the file at the provided position.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.write = function(/* UInt64 */ position, /* Byte */ section) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Write',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['UInt64'].encode( position, [] ),
            everbase.rpc.jsonEncoding._conversions['Byte'].encode( section, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Reads a block of bytes from the file at the provided position.
 */
/* Promise [UInt32 bytesRead] */ everbase.objectStore.Object.prototype.read = function(/* UInt64 */ position, /* Byte */ section) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Read',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['UInt64'].encode( position, [] ),
            everbase.rpc.jsonEncoding._conversions['Byte'].encode( section, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'UInt32' , [] ];
    });
};

/**
 * @brief Truncates the object to a specified length in bytes.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.truncate = function(/* UInt64 */ newContentSize, /* Byte */ section) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Truncate',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['UInt64'].encode( newContentSize, [] ),
            everbase.rpc.jsonEncoding._conversions['Byte'].encode( section, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the current object size in bytes.
 */
/* Promise [UInt64 curContentSize] */ everbase.objectStore.Object.prototype.getSize = function(/* Byte */ section) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::GetSize',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Byte'].encode( section, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'UInt64' , [] ];
    });
};

/**
 * @brief Commits the current state of the object as a new version.
 */
/* Promise [Uuid newVersion] */ everbase.objectStore.Object.prototype.commit = function(/* Set */ additionalParentVersions) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Commit',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Set'].encode( additionalParentVersions, ['Uuid'] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Discards all changes since the last commit or open operation.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.rollback = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Rollback',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Erases the object.
 * 
 * Once the object is erased, all subsequent calls to that object fail.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.erase = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Erase',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Closes the object.
 * 
 * Once the object is closed, all subsequent calls to that object fail.
 */
/* Promise [] */ everbase.objectStore.Object.prototype.close = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Object::Close',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief An object had been created.
 */
// event: everbase.objectStore.Object.created {

everbase.objectStore.Object.created = function() { };

everbase.objectStore.Object.created.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.Object.created.TYPE_NAME =
everbase.objectStore.Object.created.prototype.TYPE_NAME = 'Everbase::ObjectStore::Object::Created'

/**
 * @brief Id of object
 */
Object.defineProperty(everbase.objectStore.Object.created.prototype, 'id', { get: function() { return this._id }, set: function(id) { this._id = id } } ); /* Uuid */

/**
 * @brief Version of object
 */
Object.defineProperty(everbase.objectStore.Object.created.prototype, 'version', { get: function() { return this._version }, set: function(version) { this._version = version } } ); /* Uuid */

/**
 * @brief Type of object
 */
Object.defineProperty(everbase.objectStore.Object.created.prototype, 'type', { get: function() { return this._type }, set: function(type) { this._type = type } } ); /* Uuid */

// event: }

/**
 * @brief An object had been removed.
 */
// event: everbase.objectStore.Object.erased {

everbase.objectStore.Object.erased = function() { };

everbase.objectStore.Object.erased.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.Object.erased.TYPE_NAME =
everbase.objectStore.Object.erased.prototype.TYPE_NAME = 'Everbase::ObjectStore::Object::Erased'

/**
 * @brief Id of object
 */
Object.defineProperty(everbase.objectStore.Object.erased.prototype, 'id', { get: function() { return this._id }, set: function(id) { this._id = id } } ); /* Uuid */

/**
 * @brief Version of object
 */
Object.defineProperty(everbase.objectStore.Object.erased.prototype, 'version', { get: function() { return this._version }, set: function(version) { this._version = version } } ); /* Uuid */

/**
 * @brief Type of object
 */
Object.defineProperty(everbase.objectStore.Object.erased.prototype, 'type', { get: function() { return this._type }, set: function(type) { this._type = type } } ); /* Uuid */

// event: }

/**
 * @brief A new version of an object had been created.
 */
// event: everbase.objectStore.Object.newVersion {

everbase.objectStore.Object.newVersion = function() { };

everbase.objectStore.Object.newVersion.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.Object.newVersion.TYPE_NAME =
everbase.objectStore.Object.newVersion.prototype.TYPE_NAME = 'Everbase::ObjectStore::Object::NewVersion'

/**
 * @brief Id of object
 */
Object.defineProperty(everbase.objectStore.Object.newVersion.prototype, 'id', { get: function() { return this._id }, set: function(id) { this._id = id } } ); /* Uuid */

/**
 * @brief Version of object
 */
Object.defineProperty(everbase.objectStore.Object.newVersion.prototype, 'version', { get: function() { return this._version }, set: function(version) { this._version = version } } ); /* Uuid */

/**
 * @brief Type of object
 */
Object.defineProperty(everbase.objectStore.Object.newVersion.prototype, 'type', { get: function() { return this._type }, set: function(type) { this._type = type } } ); /* Uuid */

// event: }

/**
 * @brief A status update of an object occured.
 */
// event: everbase.objectStore.Object.statusUpdate {

everbase.objectStore.Object.statusUpdate = function() { };

everbase.objectStore.Object.statusUpdate.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.Object.statusUpdate.TYPE_NAME =
everbase.objectStore.Object.statusUpdate.prototype.TYPE_NAME = 'Everbase::ObjectStore::Object::StatusUpdate'

/**
 * @brief Id of object
 */
Object.defineProperty(everbase.objectStore.Object.statusUpdate.prototype, 'id', { get: function() { return this._id }, set: function(id) { this._id = id } } ); /* Uuid */

/**
 * @brief Status of object
 */
Object.defineProperty(everbase.objectStore.Object.statusUpdate.prototype, 'status', { get: function() { return this._status }, set: function(status) { this._status = status } } ); /* everbase.objectStore.status */

// event: }

// class: }

/**
 * @brief Describes a conflict which occured during a concurrent update of Everbase::ObjectStore::.
 */
// class: everbase.objectStore.Conflict {

everbase.objectStore.Conflict = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Returns all currently known conflicts.
 */
/* Promise [Set conflicts] */ everbase.objectStore.Conflict.getAll = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::GetAll',
        uuid(),
        []
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::ObjectStore::Conflict'] ];
    });
};

/**
 * @brief Opens a conflict.
 */
/* Promise [everbase.objectStore.Conflict conflict] */ everbase.objectStore.Conflict.open = function(/* Uuid */ id) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Conflict' , [] ];
    });
};

/**
 * @brief Returns the conflict id.
 */
/* Promise [Uuid id] */ everbase.objectStore.Conflict.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns the object id of the conflict-prone object.
 */
/* Promise [Uuid objectId] */ everbase.objectStore.Conflict.prototype.getObjectId = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::GetObjectId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns the version id of the first version.
 */
/* Promise [Uuid version1] */ everbase.objectStore.Conflict.prototype.getVersion1 = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::GetVersion1',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns the version id of the second version.
 */
/* Promise [Uuid version2] */ everbase.objectStore.Conflict.prototype.getVersion2 = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::GetVersion2',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Resolves the conflict by keeping one of the two conflicting versions.
 */
/* Promise [] */ everbase.objectStore.Conflict.prototype.keepSingle = function(/* Uuid */ version) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::KeepSingle',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( version, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Resolves the conflict by keeping both of the two conflicting
 *        versions. Returns the new object id of the second version.
 */
/* Promise [Uuid newObjectId] */ everbase.objectStore.Conflict.prototype.keepBoth = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::KeepBoth',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns true if the conflict is resolved.
 */
/* Promise [Boolean status] */ everbase.objectStore.Conflict.prototype.isResolved = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::Conflict::IsResolved',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief New file version conflicts had been detected.
 */
// event: everbase.objectStore.Conflict.occured {

everbase.objectStore.Conflict.occured = function() { };

everbase.objectStore.Conflict.occured.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.Conflict.occured.TYPE_NAME =
everbase.objectStore.Conflict.occured.prototype.TYPE_NAME = 'Everbase::ObjectStore::Conflict::Occured'

/**
 * @brief Set of conflict ids.
 */
Object.defineProperty(everbase.objectStore.Conflict.occured.prototype, 'conflictIds', { get: function() { return this._conflictIds }, set: function(conflictIds) { this._conflictIds = conflictIds } } ); /* Set */

// event: }

/**
 * @brief File version conflicts had been resolved.
 */
// event: everbase.objectStore.Conflict.resolved {

everbase.objectStore.Conflict.resolved = function() { };

everbase.objectStore.Conflict.resolved.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.Conflict.resolved.TYPE_NAME =
everbase.objectStore.Conflict.resolved.prototype.TYPE_NAME = 'Everbase::ObjectStore::Conflict::Resolved'

/**
 * @brief Set of conflict ids.
 */
Object.defineProperty(everbase.objectStore.Conflict.resolved.prototype, 'conflictIds', { get: function() { return this._conflictIds }, set: function(conflictIds) { this._conflictIds = conflictIds } } ); /* Set */

// event: }

// class: }

/**
 * @brief Describes a sync process of an object.
 */
// class: everbase.objectStore.SyncProcess {

everbase.objectStore.SyncProcess = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Returns all active or paused sync processes.
 */
/* Promise [Set syncProcesses] */ everbase.objectStore.SyncProcess.getAll = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::GetAll',
        uuid(),
        []
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::ObjectStore::SyncProcess'] ];
    });
};

/**
 * @brief Opens a sync process.
 */
/* Promise [everbase.objectStore.SyncProcess syncProcesses] */ everbase.objectStore.SyncProcess.open = function(/* Uuid */ id) {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::SyncProcess' , [] ];
    });
};

/**
 * @brief Returns the sync process id.
 */
/* Promise [Uuid id] */ everbase.objectStore.SyncProcess.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns the object id.
 */
/* Promise [Uuid objectId] */ everbase.objectStore.SyncProcess.prototype.getObjectId = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::GetObjectId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns the version id.
 */
/* Promise [Uuid version] */ everbase.objectStore.SyncProcess.prototype.getVersion = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::GetVersion',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns true if the sync process is currently paused.
 */
/* Promise [Boolean paused] */ everbase.objectStore.SyncProcess.prototype.isPaused = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::IsPaused',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Returns the current sync status.
 */
/* Promise [everbase.objectStore.syncStatus syncStatus] */ everbase.objectStore.SyncProcess.prototype.getStatus = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::GetStatus',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::SyncStatus' , [] ];
    });
};

/**
 * @brief Pauses the sync process.
 */
/* Promise [] */ everbase.objectStore.SyncProcess.prototype.pause = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::Pause',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Resumes the sync process.
 */
/* Promise [] */ everbase.objectStore.SyncProcess.prototype.resume = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::Resume',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Cancels the sync process.
 * 
 * In case the sync process was triggered by an operation on an object
 * which requires the data to be synchronized, this operation may fail.
 */
/* Promise [] */ everbase.objectStore.SyncProcess.prototype.cancel = function() {
    var message = 
    [
        'call',
        'Everbase::ObjectStore::SyncProcess::Cancel',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief A new sync process started.
 */
// event: everbase.objectStore.SyncProcess.started {

everbase.objectStore.SyncProcess.started = function() { };

everbase.objectStore.SyncProcess.started.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.SyncProcess.started.TYPE_NAME =
everbase.objectStore.SyncProcess.started.prototype.TYPE_NAME = 'Everbase::ObjectStore::SyncProcess::Started'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.objectStore.SyncProcess.started.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

/**
 * @brief An existing sync process completed.
 */
// event: everbase.objectStore.SyncProcess.completed {

everbase.objectStore.SyncProcess.completed = function() { };

everbase.objectStore.SyncProcess.completed.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.SyncProcess.completed.TYPE_NAME =
everbase.objectStore.SyncProcess.completed.prototype.TYPE_NAME = 'Everbase::ObjectStore::SyncProcess::Completed'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.objectStore.SyncProcess.completed.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

/**
 * @brief An existing sync process had been paused.
 */
// event: everbase.objectStore.SyncProcess.paused {

everbase.objectStore.SyncProcess.paused = function() { };

everbase.objectStore.SyncProcess.paused.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.SyncProcess.paused.TYPE_NAME =
everbase.objectStore.SyncProcess.paused.prototype.TYPE_NAME = 'Everbase::ObjectStore::SyncProcess::Paused'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.objectStore.SyncProcess.paused.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

/**
 * @brief An existing sync process had been resumed.
 */
// event: everbase.objectStore.SyncProcess.resumed {

everbase.objectStore.SyncProcess.resumed = function() { };

everbase.objectStore.SyncProcess.resumed.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.SyncProcess.resumed.TYPE_NAME =
everbase.objectStore.SyncProcess.resumed.prototype.TYPE_NAME = 'Everbase::ObjectStore::SyncProcess::Resumed'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.objectStore.SyncProcess.resumed.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

/**
 * @brief An existing sync process made progress.
 */
// event: everbase.objectStore.SyncProcess.progress {

everbase.objectStore.SyncProcess.progress = function() { };

everbase.objectStore.SyncProcess.progress.prototype = Object.create(everbase.Event.prototype);

everbase.objectStore.SyncProcess.progress.TYPE_NAME =
everbase.objectStore.SyncProcess.progress.prototype.TYPE_NAME = 'Everbase::ObjectStore::SyncProcess::Progress'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.objectStore.SyncProcess.progress.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

// class: }

/**
 * @brief Everbase File System.
 */
everbase.fileSystem = everbase.fileSystem || { };

/**
 * @brief Specifies a unique location of a directory or file within the EFS.
 * 
 * A path is a sequence of names. A name can be any valid unicode string.
 * Each name can be associated with a version identifier.
 */
// class: everbase.fileSystem.Path {

everbase.fileSystem.Path = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Converts a system path to a canonical path object.
 * 
 * The elements of a system path are either seperated by slashes or
 * backslashes. Conversion depends on device local information, e.g.
 * system user local mount point of EFS.
 */
/* Promise [everbase.fileSystem.Path path] */ everbase.fileSystem.Path.fromSysPath = function(/* String */ systemPath) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::FromSysPath',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( systemPath, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Converts a web url, to a canonical path object.
 * 
 * The elements of a web url are seperated by slashes. Elements of a web
 * url can be annotated with version information. Version information is
 * separated by a colon. Conversion depends on domain local information,
 * e.g. url of web interface.
 */
/* Promise [everbase.fileSystem.Path path] */ everbase.fileSystem.Path.fromWebUrl = function(/* String */ webUrl) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::FromWebUrl',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( webUrl, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Converts a canonical URL to a canonical path object.
 * 
 * The protocol of an EFS URL is everbase. The elements of an URL are
 * seperated by slashes. Elements of an URL can be annotated with version
 * information. Version information is separated by a colon. No additional
 * information required for conversion.
 */
/* Promise [everbase.fileSystem.Path path] */ everbase.fileSystem.Path.fromEverbaseUrl = function(/* String */ everbaseUrl) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::FromEverbaseUrl',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( everbaseUrl, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Converts a canonical path object to a system path. See FromSysPath.
 */
/* Promise [String sysPath] */ everbase.fileSystem.Path.prototype.toSysPath = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::ToSysPath',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Converts a canonical path object to a web url. See FromWebUrl.
 */
/* Promise [String webUrl] */ everbase.fileSystem.Path.prototype.toWebUrl = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::ToWebUrl',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Converts a canonical path object to a canonical URL. See FromEverbaseUrl.
 */
/* Promise [String everbaseUrl] */ everbase.fileSystem.Path.prototype.toEverbaseUrl = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::ToEverbaseUrl',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Returns true if provided string is a valid system path. See FromSysPath.
 */
/* Promise [Boolean validSysPath] */ everbase.fileSystem.Path.isValidSysPath = function(/* String */ sysPath) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::IsValidSysPath',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( sysPath, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Returns true if provided string is a valid web url. See FromWebUrl.
 */
/* Promise [Boolean validWebUrl] */ everbase.fileSystem.Path.isValidWebUrl = function(/* String */ webUrl) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::IsValidWebUrl',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( webUrl, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Returns true if provided string is a valid everbase url. See FromEverbaseUrl.
 */
/* Promise [Boolean validEverbaseUrl] */ everbase.fileSystem.Path.isValidEverbaseUrl = function(/* String */ everbaseUrl) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::IsValidEverbaseUrl',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( everbaseUrl, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Returns true in case we have a root path.
 */
/* Promise [Boolean root] */ everbase.fileSystem.Path.prototype.isRoot = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::IsRoot',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Returns the parent path.
 */
/* Promise [everbase.fileSystem.Path parent] */ everbase.fileSystem.Path.prototype.getParent = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::GetParent',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Returns a child path.
 */
/* Promise [everbase.fileSystem.Path child] */ everbase.fileSystem.Path.prototype.getChild = function(/* String */ childName) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::GetChild',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( childName, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Returns a versioned child path.
 */
/* Promise [everbase.fileSystem.Path child] */ everbase.fileSystem.Path.prototype.getVersionedChild = function(/* String */ childName, /* Uuid */ childVersion) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::GetVersionedChild',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( childName, [] ),
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( childVersion, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Returns the name of the specified location.
 */
/* Promise [String name] */ everbase.fileSystem.Path.prototype.getName = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::GetName',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'String' , [] ];
    });
};

/**
 * @brief Returns the version of the specified location.
 */
/* Promise [Uuid version] */ everbase.fileSystem.Path.prototype.getVersion = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::GetVersion',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns true if the the specified location has version information.
 */
/* Promise [Boolean versionStatus] */ everbase.fileSystem.Path.prototype.hasVersion = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Path::HasVersion',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

// class: }

/**
 * @brief Is a key value store.
 * 
 * The key value store can be used by the application developers to save
 * small pieces of data with arbitrary type.
 */
// class: everbase.fileSystem.Metadata {

everbase.fileSystem.Metadata = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Associates the specified value with the specified key in the key
 *        value store.
 * 
 * If the store previously contained the key, the old value is replaced by
 * the specified value.
 */
/* Promise [] */ everbase.fileSystem.Metadata.prototype.setValue = function(/* String */ key, /* Vector */ value) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Metadata::SetValue',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( key, [] ),
            everbase.rpc.jsonEncoding._conversions['Vector'].encode( value, ['Byte'] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the value to which the specified key is associated.
 * 
 * Failes in case the store does not contain the specified key.
 */
/* Promise [Vector value] */ everbase.fileSystem.Metadata.prototype.getValue = function(/* String */ key) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Metadata::GetValue',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( key, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Vector' , ['Byte'] ];
    });
};

/**
 * @brief Returns true if this key value store contains the specified key.
 */
/* Promise [Boolean exists] */ everbase.fileSystem.Metadata.prototype.has = function(/* String */ key) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Metadata::Has',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( key, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Erases the specified key.
 */
/* Promise [] */ everbase.fileSystem.Metadata.prototype.erase = function(/* String */ key) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Metadata::Erase',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['String'].encode( key, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns all keys of the key value store.
 */
/* Promise [Set keys] */ everbase.fileSystem.Metadata.prototype.getKeys = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Metadata::GetKeys',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['String'] ];
    });
};

// class: }

/**
 * @brief Is a filesystem node, which can be one of various things
 *        including a file or directory.
 */
// class: everbase.fileSystem.Node {

everbase.fileSystem.Node = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Moves a specified filesystem node to a new location, providing
 *        the option to specify a new file name.
 */
/* Promise [] */ everbase.fileSystem.Node.move = function(/* everbase.fileSystem.Path */ sourcePath, /* everbase.fileSystem.Path */ targetPath) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::Move',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( sourcePath, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( targetPath, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Copies an existing filesystem node to a new node. Overwriting a
 *        node of the same name is not allowed.
 */
/* Promise [] */ everbase.fileSystem.Node.copy = function(/* everbase.fileSystem.Path */ sourcePath, /* everbase.fileSystem.Path */ targetPath, /* Boolean */ copyHistory) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::Copy',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( sourcePath, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( targetPath, [] ),
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( copyHistory, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Deletes the specified filesystem node.
 */
/* Promise [] */ everbase.fileSystem.Node.remove = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::Remove',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns status information of a filesystem node.
 */
/* Promise [everbase.objectStore.status status] */ everbase.fileSystem.Node.getStatus = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetStatus',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Status' , [] ];
    });
};

/**
 * @brief Returns status information of filesystem nodes recursively.
 */
/* Promise [everbase.objectStore.cumulatedStatus status] */ everbase.fileSystem.Node.getTreeStatus = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetTreeStatus',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::CumulatedStatus' , [] ];
    });
};

/**
 * @brief Sets the pinning status of the path.
 */
/* Promise [] */ everbase.fileSystem.Node.setPinning = function(/* everbase.fileSystem.Path */ path, /* Boolean */ pinning) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::SetPinning',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( pinning, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Gets the pinning status of the path.
 */
/* Promise [Boolean pinning] */ everbase.fileSystem.Node.getPinning = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetPinning',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Closed the filesystem node.
 * 
 * Once the filesystem node is closed, all subsequent calls to that object fail.
 */
/* Promise [] */ everbase.fileSystem.Node.prototype.close = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::Close',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Commits the current state of the node as a new version.
 * 
 * Do not commit for no reason.
 */
/* Promise [everbase.fileSystem.Path newVersionedPath] */ everbase.fileSystem.Node.prototype.commit = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::Commit',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Discards all changes since the last commit or open operation.
 */
/* Promise [] */ everbase.fileSystem.Node.prototype.rollback = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::Rollback',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the metadata object of the filesystem node.
 */
/* Promise [everbase.fileSystem.Metadata metadata] */ everbase.fileSystem.Node.prototype.getMetadata = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetMetadata',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Metadata' , [] ];
    });
};

/**
 * @brief Returns the unversioned path of the filesystem node.
 */
/* Promise [everbase.fileSystem.Path path] */ everbase.fileSystem.Node.prototype.getPath = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetPath',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Returns the fully versioned path of this specific filesystem
 *        node version.
 * 
 * Some Directories may be unversioned.
 */
/* Promise [everbase.fileSystem.Path versionedPath] */ everbase.fileSystem.Node.prototype.getVersionedPath = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetVersionedPath',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Returns the versioned paths of the parent versions.
 */
/* Promise [Set versionedPaths] */ everbase.fileSystem.Node.prototype.getParentVersions = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetParentVersions',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::FileSystem::Path'] ];
    });
};

/**
 * @brief Attaches a policy to the filesystem node.
 */
/* Promise [] */ everbase.fileSystem.Node.prototype.attachPolicy = function(/* everbase.objectStore.Policy */ policy, /* everbase.objectStore.Policy */ after) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::AttachPolicy',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Policy'].encode( policy, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Policy'].encode( after, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Detaches a policy from the filesystem node.
 */
/* Promise [] */ everbase.fileSystem.Node.prototype.detachPolicy = function(/* everbase.objectStore.Policy */ policy) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::DetachPolicy',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::ObjectStore::Policy'].encode( policy, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns all attached policies of the filesystem node.
 */
/* Promise [Set policies] */ everbase.fileSystem.Node.prototype.getPolicies = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetPolicies',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::ObjectStore::Policy'] ];
    });
};

/**
 * @brief Returns the effective policy of the filesystem node.
 * 
 * The effective policy is a computed object derived from all applicable
 * attached and inherited policies.
 */
/* Promise [everbase.objectStore.Policy policy] */ everbase.fileSystem.Node.prototype.getEffectivePolicy = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetEffectivePolicy',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::Policy' , [] ];
    });
};

/**
 * @brief Returns true if policy inheritance is activated (default).
 */
/* Promise [Boolean inheritance] */ everbase.fileSystem.Node.prototype.getPolicyInheritance = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::GetPolicyInheritance',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Enables or disables policy inheritance.
 */
/* Promise [] */ everbase.fileSystem.Node.prototype.setPolicyInheritance = function(/* Boolean */ inheritance) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Node::SetPolicyInheritance',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( inheritance, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief A filesystem node had been created.
 */
// event: everbase.fileSystem.Node.created {

everbase.fileSystem.Node.created = function() { };

everbase.fileSystem.Node.created.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.Node.created.TYPE_NAME =
everbase.fileSystem.Node.created.prototype.TYPE_NAME = 'Everbase::FileSystem::Node::Created'

/**
 * @brief Path of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.created.prototype, 'path', { get: function() { return this._path }, set: function(path) { this._path = path } } ); /* everbase.fileSystem.Path */

// event: }

/**
 * @brief A filesystem node had been moved or renamed.
 */
// event: everbase.fileSystem.Node.moved {

everbase.fileSystem.Node.moved = function() { };

everbase.fileSystem.Node.moved.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.Node.moved.TYPE_NAME =
everbase.fileSystem.Node.moved.prototype.TYPE_NAME = 'Everbase::FileSystem::Node::Moved'

/**
 * @brief Source path of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.moved.prototype, 'sourcePath', { get: function() { return this._sourcePath }, set: function(sourcePath) { this._sourcePath = sourcePath } } ); /* everbase.fileSystem.Path */

/**
 * @brief Target path of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.moved.prototype, 'targetPath', { get: function() { return this._targetPath }, set: function(targetPath) { this._targetPath = targetPath } } ); /* everbase.fileSystem.Path */

// event: }

/**
 * @brief A filesystem node had been copied.
 */
// event: everbase.fileSystem.Node.copied {

everbase.fileSystem.Node.copied = function() { };

everbase.fileSystem.Node.copied.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.Node.copied.TYPE_NAME =
everbase.fileSystem.Node.copied.prototype.TYPE_NAME = 'Everbase::FileSystem::Node::Copied'

/**
 * @brief Source path of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.copied.prototype, 'sourcePath', { get: function() { return this._sourcePath }, set: function(sourcePath) { this._sourcePath = sourcePath } } ); /* everbase.fileSystem.Path */

/**
 * @brief Target path of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.copied.prototype, 'targetPath', { get: function() { return this._targetPath }, set: function(targetPath) { this._targetPath = targetPath } } ); /* everbase.fileSystem.Path */

// event: }

/**
 * @brief A filesystem node had been removed.
 */
// event: everbase.fileSystem.Node.removed {

everbase.fileSystem.Node.removed = function() { };

everbase.fileSystem.Node.removed.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.Node.removed.TYPE_NAME =
everbase.fileSystem.Node.removed.prototype.TYPE_NAME = 'Everbase::FileSystem::Node::Removed'

/**
 * @brief Path of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.removed.prototype, 'path', { get: function() { return this._path }, set: function(path) { this._path = path } } ); /* everbase.fileSystem.Path */

// event: }

/**
 * @brief A new version of a filesystem node had been created.
 */
// event: everbase.fileSystem.Node.newVersion {

everbase.fileSystem.Node.newVersion = function() { };

everbase.fileSystem.Node.newVersion.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.Node.newVersion.TYPE_NAME =
everbase.fileSystem.Node.newVersion.prototype.TYPE_NAME = 'Everbase::FileSystem::Node::NewVersion'

/**
 * @brief Path of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.newVersion.prototype, 'versionedPath', { get: function() { return this._versionedPath }, set: function(versionedPath) { this._versionedPath = versionedPath } } ); /* everbase.fileSystem.Path */

// event: }

/**
 * @brief A status update of a filesystem node occured.
 */
// event: everbase.fileSystem.Node.statusUpdate {

everbase.fileSystem.Node.statusUpdate = function() { };

everbase.fileSystem.Node.statusUpdate.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.Node.statusUpdate.TYPE_NAME =
everbase.fileSystem.Node.statusUpdate.prototype.TYPE_NAME = 'Everbase::FileSystem::Node::StatusUpdate'

/**
 * @brief Path of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.statusUpdate.prototype, 'path', { get: function() { return this._path }, set: function(path) { this._path = path } } ); /* everbase.fileSystem.Path */

/**
 * @brief Status of filesystem node.
 */
Object.defineProperty(everbase.fileSystem.Node.statusUpdate.prototype, 'status', { get: function() { return this._status }, set: function(status) { this._status = status } } ); /* everbase.objectStore.status */

// event: }

// class: }

/**
 * @brief Is a file system cataloging structure which contains references
 *        to other files and directories.
 * 
 * Each entry of an directory is associated with an arbitrary name. User
 * space filesystem implementation have to handle the translation of these
 * names to operating system compatible names.
 */
// class: everbase.fileSystem.Directory {

everbase.fileSystem.Directory = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

everbase.fileSystem.Directory.prototype = Object.create(everbase.fileSystem.Node.prototype);

/**
 * @brief Object type identifier for directories.
 */
everbase.fileSystem.Directory.TYPEID = [ 0x5f, 0x58, 0xa9, 0x8, 0x6f, 0xcf, 0x4b, 0xf2, 0xaa, 0xf3, 0xa0, 0x6c, 0x75, 0x72, 0xfe, 0xc5 ];

/**
 * @brief Creates a new directory.
 */
/* Promise [everbase.fileSystem.Directory directory] */ everbase.fileSystem.Directory.create = function(/* String */ name, /* everbase.fileSystem.Path */ parent) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Directory::Create',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( parent, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Directory' , [] ];
    });
};

/**
 * @brief Opens an existing directory.
 */
/* Promise [everbase.fileSystem.Directory directory] */ everbase.fileSystem.Directory.open = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Directory::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Directory' , [] ];
    });
};

/**
 * @brief Returns all directory entries presented as a set of paths.
 */
/* Promise [Set entries] */ everbase.fileSystem.Directory.prototype.getEntries = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Directory::GetEntries',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::FileSystem::Path'] ];
    });
};

/**
 * @brief Returns the count of directory entries.
 */
/* Promise [UInt64 entryCount] */ everbase.fileSystem.Directory.prototype.getEntryCount = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Directory::GetEntryCount',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'UInt64' , [] ];
    });
};

// class: }

/**
 * @brief Represents the root directory of a profile.
 */
// class: everbase.fileSystem.ProfileRoot {

everbase.fileSystem.ProfileRoot = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

everbase.fileSystem.ProfileRoot.prototype = Object.create(everbase.fileSystem.Directory.prototype);

/**
 * @brief Profile root type identifier for directories.
 */
everbase.fileSystem.ProfileRoot.TYPEID = [ 0xc4, 0xc3, 0x91, 0xc4, 0xe5, 0x30, 0x4f, 0xeb, 0xaa, 0xb7, 0x9a, 0x12, 0xd3, 0x71, 0x5f, 0x6e ];

/**
 * @brief Opens the root of the current profile.
 */
/* Promise [everbase.fileSystem.ProfileRoot profileRoot] */ everbase.fileSystem.ProfileRoot.open = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::ProfileRoot::Open',
        uuid(),
        []
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::ProfileRoot' , [] ];
    });
};

/**
 * @brief Returns the associated profile.
 */
/* Promise [everbase.coreSystem.Profile profile] */ everbase.fileSystem.ProfileRoot.prototype.getProfile = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::ProfileRoot::GetProfile',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Profile' , [] ];
    });
};

// class: }

/**
 * @brief Represents the root directory of a domain.
 */
// class: everbase.fileSystem.DomainRoot {

everbase.fileSystem.DomainRoot = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

everbase.fileSystem.DomainRoot.prototype = Object.create(everbase.fileSystem.Directory.prototype);

/**
 * @brief Domain root type identifier for directories.
 */
everbase.fileSystem.DomainRoot.TYPEID = [ 0x7d, 0xd6, 0x9f, 0xdd, 0x9b, 0xc9, 0x43, 0x22, 0xba, 0xbc, 0x62, 0xd6, 0x14, 0x3f, 0xd7, 0x52 ];

/**
 * @brief Opens the root of an domain. We have to be authenticated to that domain.
 */
/* Promise [everbase.fileSystem.DomainRoot domainRoot] */ everbase.fileSystem.DomainRoot.open = function(/* String */ domainName) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::DomainRoot::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( domainName, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::DomainRoot' , [] ];
    });
};

/**
 * @brief Returns the associated domain.
 */
/* Promise [everbase.coreSystem.Domain domain] */ everbase.fileSystem.DomainRoot.prototype.getDomain = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::DomainRoot::GetDomain',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::CoreSystem::Domain' , [] ];
    });
};

// class: }

/**
 * @brief Is a file system storage structure which can be used to store
 *        arbitrary data.
 */
// class: everbase.fileSystem.File {

everbase.fileSystem.File = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

everbase.fileSystem.File.prototype = Object.create(everbase.fileSystem.Node.prototype);

/**
 * @brief File type identifier for directories.
 */
everbase.fileSystem.File.TYPEID = [ 0xbf, 0x86, 0x92, 0x64, 0x18, 0x88, 0x45, 0x16, 0xa6, 0xf6, 0x89, 0x0, 0x75, 0xf7, 0xf6, 0x57 ];

/**
 * @brief Creates a new file.
 */
/* Promise [everbase.fileSystem.File file] */ everbase.fileSystem.File.create = function(/* String */ name, /* everbase.fileSystem.Path */ parent) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::Create',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['String'].encode( name, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( parent, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::File' , [] ];
    });
};

/**
 * @brief Opens an existing file.
 */
/* Promise [everbase.fileSystem.File file] */ everbase.fileSystem.File.open = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::File' , [] ];
    });
};

/**
 * @brief Informs the system about the fact that the local user is viewing
 *        the file.
 */
/* Promise [UInt32 userViewId] */ everbase.fileSystem.File.beginUserView = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::BeginUserView',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'UInt32' , [] ];
    });
};

/**
 * @brief Informs the system about the fact that the local user stopped
 *        viewing the file.
 */
/* Promise [] */ everbase.fileSystem.File.endUserView = function(/* UInt32 */ userViewId) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::EndUserView',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['UInt32'].encode( userViewId, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Informs the system about the fact that the local user is editing
 *        the file.
 */
/* Promise [UInt32 userEditId] */ everbase.fileSystem.File.beginUserEdit = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::BeginUserEdit',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'UInt32' , [] ];
    });
};

/**
 * @brief Informs the system about the fact that the local user stopped
 *        editing the file.
 */
/* Promise [] */ everbase.fileSystem.File.endUserEdit = function(/* UInt32 */ userEditId) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::EndUserEdit',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['UInt32'].encode( userEditId, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Writes a block of bytes to the file at the provided position.
 */
/* Promise [] */ everbase.fileSystem.File.prototype.write = function(/* UInt64 */ position) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::Write',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['UInt64'].encode( position, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Reads a block of bytes from the file at the provided position.
 */
/* Promise [UInt32 bytesRead] */ everbase.fileSystem.File.prototype.read = function(/* Boolean */ fillBuffer, /* UInt64 */ position) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::Read',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( fillBuffer, [] ),
            everbase.rpc.jsonEncoding._conversions['UInt64'].encode( position, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'UInt32' , [] ];
    });
};

/**
 * @brief Truncates a file to a specified length in bytes.
 */
/* Promise [] */ everbase.fileSystem.File.prototype.truncate = function(/* UInt64 */ newFileSize) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::Truncate',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['UInt64'].encode( newFileSize, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns the current file size in bytes.
 */
/* Promise [UInt64 curFileSize] */ everbase.fileSystem.File.prototype.getSize = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::File::GetSize',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'UInt64' , [] ];
    });
};

// class: }

/**
 * @brief Describes a conflict which occured during a concurrent update of files.
 */
// class: everbase.fileSystem.Conflict {

everbase.fileSystem.Conflict = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Returns all currently known conflicts for a provided path.
 */
/* Promise [Set conflicts] */ everbase.fileSystem.Conflict.getAll = function(/* everbase.fileSystem.Path */ path, /* Boolean */ recursive) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Conflict::GetAll',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( recursive, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::FileSystem::Conflict'] ];
    });
};

/**
 * @brief Opens a conflict.
 */
/* Promise [everbase.fileSystem.Conflict conflict] */ everbase.fileSystem.Conflict.open = function(/* Uuid */ id) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Conflict::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Conflict' , [] ];
    });
};

/**
 * @brief Returns the conflict id.
 */
/* Promise [Uuid id] */ everbase.fileSystem.Conflict.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Conflict::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns the first path of the conflicting versions.
 */
/* Promise [everbase.fileSystem.Path versionedPath1] */ everbase.fileSystem.Conflict.prototype.getPath1 = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Conflict::GetPath1',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Returns the second path of the conflicting versions.
 */
/* Promise [everbase.fileSystem.Path versionedPath2] */ everbase.fileSystem.Conflict.prototype.getPath2 = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Conflict::GetPath2',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Resolves the conflict by keeping one of the two conflicting versions.
 */
/* Promise [] */ everbase.fileSystem.Conflict.prototype.keepSingle = function(/* everbase.fileSystem.Path */ path) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Conflict::KeepSingle',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Resolves the conflict by keeping both of the two conflicting versions.
 */
/* Promise [] */ everbase.fileSystem.Conflict.prototype.keepBoth = function(/* everbase.fileSystem.Path */ path1, /* everbase.fileSystem.Path */ path2) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Conflict::KeepBoth',
        uuid(),
        [
            this._handle,
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path1, [] ),
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path2, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Returns true if the conflict is resolved.
 */
/* Promise [Boolean resolved] */ everbase.fileSystem.Conflict.prototype.isResolved = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::Conflict::IsResolved',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief New file version conflicts had been detected.
 */
// event: everbase.fileSystem.Conflict.occured {

everbase.fileSystem.Conflict.occured = function() { };

everbase.fileSystem.Conflict.occured.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.Conflict.occured.TYPE_NAME =
everbase.fileSystem.Conflict.occured.prototype.TYPE_NAME = 'Everbase::FileSystem::Conflict::Occured'

/**
 * @brief Set of conflict ids.
 */
Object.defineProperty(everbase.fileSystem.Conflict.occured.prototype, 'conflictIds', { get: function() { return this._conflictIds }, set: function(conflictIds) { this._conflictIds = conflictIds } } ); /* Set */

// event: }

/**
 * @brief File version conflicts had been resolved.
 */
// event: everbase.fileSystem.Conflict.resolved {

everbase.fileSystem.Conflict.resolved = function() { };

everbase.fileSystem.Conflict.resolved.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.Conflict.resolved.TYPE_NAME =
everbase.fileSystem.Conflict.resolved.prototype.TYPE_NAME = 'Everbase::FileSystem::Conflict::Resolved'

/**
 * @brief Set of conflict ids.
 */
Object.defineProperty(everbase.fileSystem.Conflict.resolved.prototype, 'conflictIds', { get: function() { return this._conflictIds }, set: function(conflictIds) { this._conflictIds = conflictIds } } ); /* Set */

// event: }

// class: }

/**
 * @brief Describes a sync process of a file or folder.
 */
// class: everbase.fileSystem.SyncProcess {

everbase.fileSystem.SyncProcess = function(handle) { this._handle = typeof handle !== 'undefined' ? handle : null };

/**
 * @brief Returns all active or paused sync processes for a path.
 */
/* Promise [Set syncProcesses] */ everbase.fileSystem.SyncProcess.getAll = function(/* everbase.fileSystem.Path */ path, /* Boolean */ recursive) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::GetAll',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Everbase::FileSystem::Path'].encode( path, [] ),
            everbase.rpc.jsonEncoding._conversions['Boolean'].encode( recursive, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Set' , ['Everbase::FileSystem::SyncProcess'] ];
    });
};

/**
 * @brief Opens a sync process.
 */
/* Promise [everbase.fileSystem.SyncProcess syncProcesses] */ everbase.fileSystem.SyncProcess.open = function(/* Uuid */ id) {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::Open',
        uuid(),
        [
            everbase.rpc.jsonEncoding._conversions['Uuid'].encode( id, [] ),
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::SyncProcess' , [] ];
    });
};

/**
 * @brief Returns the sync process id.
 */
/* Promise [Uuid id] */ everbase.fileSystem.SyncProcess.prototype.getId = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::GetId',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Uuid' , [] ];
    });
};

/**
 * @brief Returns the versioned path of the file or folder.
 */
/* Promise [everbase.fileSystem.Path path] */ everbase.fileSystem.SyncProcess.prototype.getPath = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::GetPath',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::FileSystem::Path' , [] ];
    });
};

/**
 * @brief Returns true if the sync process is currently paused.
 */
/* Promise [Boolean paused] */ everbase.fileSystem.SyncProcess.prototype.isPaused = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::IsPaused',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Boolean' , [] ];
    });
};

/**
 * @brief Returns the current sync status.
 */
/* Promise [everbase.objectStore.syncStatus syncStatus] */ everbase.fileSystem.SyncProcess.prototype.getStatus = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::GetStatus',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, 'Everbase::ObjectStore::SyncStatus' , [] ];
    });
};

/**
 * @brief Pauses the sync process.
 */
/* Promise [] */ everbase.fileSystem.SyncProcess.prototype.pause = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::Pause',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Resumes the sync process.
 */
/* Promise [] */ everbase.fileSystem.SyncProcess.prototype.resume = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::Resume',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief Cancels the sync process.
 * 
 * In case the sync process was triggered by a filesystem operation which
 * requires the data to be synchronized, this operation may fail.
 */
/* Promise [] */ everbase.fileSystem.SyncProcess.prototype.cancel = function() {
    var message = 
    [
        'call',
        'Everbase::FileSystem::SyncProcess::Cancel',
        uuid(),
        [
            this._handle,
        ]
    ];
    everbase.webclient.connection.send(message);
    
    return new Promise(function (resolve, reject) {
        everbase.webclient.processes[message[2]] = [ resolve, '', [] ];
    });
};

/**
 * @brief A new sync process started.
 */
// event: everbase.fileSystem.SyncProcess.started {

everbase.fileSystem.SyncProcess.started = function() { };

everbase.fileSystem.SyncProcess.started.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.SyncProcess.started.TYPE_NAME =
everbase.fileSystem.SyncProcess.started.prototype.TYPE_NAME = 'Everbase::FileSystem::SyncProcess::Started'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.fileSystem.SyncProcess.started.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

/**
 * @brief An existing sync process is completed.
 */
// event: everbase.fileSystem.SyncProcess.completed {

everbase.fileSystem.SyncProcess.completed = function() { };

everbase.fileSystem.SyncProcess.completed.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.SyncProcess.completed.TYPE_NAME =
everbase.fileSystem.SyncProcess.completed.prototype.TYPE_NAME = 'Everbase::FileSystem::SyncProcess::Completed'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.fileSystem.SyncProcess.completed.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

/**
 * @brief An existing sync process had been paused.
 */
// event: everbase.fileSystem.SyncProcess.paused {

everbase.fileSystem.SyncProcess.paused = function() { };

everbase.fileSystem.SyncProcess.paused.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.SyncProcess.paused.TYPE_NAME =
everbase.fileSystem.SyncProcess.paused.prototype.TYPE_NAME = 'Everbase::FileSystem::SyncProcess::Paused'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.fileSystem.SyncProcess.paused.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

/**
 * @brief An existing sync process had been resumed.
 */
// event: everbase.fileSystem.SyncProcess.resumed {

everbase.fileSystem.SyncProcess.resumed = function() { };

everbase.fileSystem.SyncProcess.resumed.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.SyncProcess.resumed.TYPE_NAME =
everbase.fileSystem.SyncProcess.resumed.prototype.TYPE_NAME = 'Everbase::FileSystem::SyncProcess::Resumed'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.fileSystem.SyncProcess.resumed.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

/**
 * @brief An existing sync process made progress.
 */
// event: everbase.fileSystem.SyncProcess.progress {

everbase.fileSystem.SyncProcess.progress = function() { };

everbase.fileSystem.SyncProcess.progress.prototype = Object.create(everbase.Event.prototype);

everbase.fileSystem.SyncProcess.progress.TYPE_NAME =
everbase.fileSystem.SyncProcess.progress.prototype.TYPE_NAME = 'Everbase::FileSystem::SyncProcess::Progress'

/**
 * @brief Sync process id.
 */
Object.defineProperty(everbase.fileSystem.SyncProcess.progress.prototype, 'syncProcessId', { get: function() { return this._syncProcessId }, set: function(syncProcessId) { this._syncProcessId = syncProcessId } } ); /* Uuid */

// event: }

// class: }

