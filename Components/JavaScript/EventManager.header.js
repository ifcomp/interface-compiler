/*************************************************************************
 *
 * @version 0.1
 * 
 * EVERBASE CONFIDENTIAL
 * __________________
 * 
 *  [2014] EVB Everbase AG, Hamburg
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains the property
 * of the EVB Everbase AG and its suppliers, if any. The intellectual and
 * technical concepts contained herein are proprietary to EVB Everbase AG
 * and its suppliers and may be covered by German and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from
 * EVB Everbase AG.
 */

/**
 * @brief Everbase namespace
 */
var Everbase = Everbase || {};


/**
 * @brief Represents an event.
 */
Everbase.Event = function() { };


/**
 * @brief Represents an observable object. An observable object can have one or more observers.
 */
Everbase.EventManager = function() { };

/**
 * @brief Fires an event.
 */
/* void */ Everbase.EventManager.fireEvent = function(/*Everbase.Event*/ event) { /* impl */ };

/**
 * @brief Adds an entry to the event dispatch table with an observer and callback. Returns an observer entry id.
 */
/* Number */ Everbase.EventManager.addObserver = function(/*Object*/ observer, /*Function*/ callback, /*Type of Everbase.Event*/ eventType) { /* impl */ };

/**
 * @brief Removes an observer entry from the event dispatch table.
 */
/* void */ Everbase.EventManager.removeObserver = function(/*Number*/ entryId) { /* impl */ };
