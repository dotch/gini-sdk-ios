/*
 *  Copyright (c) 2014, Gini GmbH.
 *  All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "GINIAPIManager.h"


@class BFTask;
@class GINIDocument;
@class GINIExtraction;


/**
 * The `GINIDocumentTaskManager` is a high level API on top of the Gini API. It provides high level methods to easily
 * handle document related tasks.
 *
 * Usage of promises/tasks
 * -----------------------
 * This manager makes heavy use of the concept of tasks (also known as promises in other languages). The Gini SDK uses
 * [Facebook's Bolts task implementation](https://github.com/BoltsFramework/Bolts-iOS#tasks). See the
 * [programming guide for tasks](../docs/2.%20Working%20with%20tasks.html) for a detailed introduction
 * into the usage of tasks in the Gini SDK.
 */
@interface GINIDocumentTaskManager : NSObject

/**
 * Factory to create a new instance of the document task manager.
 *
 * @param apiManager    An instance of `GINIAPIManager` which will be used to communicate with the Gini API.
 */
+ (instancetype)documentTaskManagerWithAPIManager:(GINIAPIManager *)apiManager;

/**
 * The designated initializer.
 *
 * @param apiManager    An instance of `GINIAPIManager` which will be used to communicate with the Gini API.
 */
- (instancetype)initWithAPIManager:(GINIAPIManager *)apiManager;

/**
 * The time in seconds between HTTP requests when polling documents.
 */
@property NSUInteger pollingInterval;

/**
 * Gets the document with the given id.
 *
 * @param documentId    The document's unique identifier.
 *
 * @returns             A `BFTask` that will resolve to a `GINIDocument` instance representing the document.
 */
- (BFTask *)getDocumentWithId:(NSString *)documentId;

/**
 * Creates a new document from the given image.
 *
 * @param fileName      The file name of the document.
 * @param image         An image representing the document.
 *
 * @returns             A `BFTask*` that will resolve to a `GINIDocument` instance representing the created document.
 *                      Please notice that it is very unlikely that the created document is already fully processed, so
 *                      the extractions may not already exist.
 */
- (BFTask *)createDocumentWithFilename:(NSString *)fileName fromImage:(UIImage *)image;

/**
 * Creates a new document with the given `doctype` from the given image. By providing the doctype, Gini's document
 * processing is optimized in many ways.
 *
 * See the [Gini API documentation](http://developer.gini.net/gini-api/html/documents.html#document-type-hints) for
 * details and [a list of available doctypes](http://developer.gini.net/gini-api/html/entity_reference.html#extraction-entity-doctype).
 *
 * @warning Some incubating extractions are only available if you create the document with this method, so the Gini API
 * knows the doctype.
 */
- (BFTask *)createDocumentWithFilename:(NSString *)fileName fromImage:(UIImage *)image docType:(NSString *)docType;

/**
 * Saves updates on the extractions.
 *
 * Updating extractions is called "Submitting feedback" in the Gini API documentation.
 *
 * @param document      The document.
 */
- (BFTask *)updateDocument:(GINIDocument *)document;

/**
 * Deletes the given document.
 *
 * @param document      The document that will be deleted.
 */
- (BFTask *)deleteDocument:(GINIDocument *)document;

/**
 * Continually checks the document status until the document is fully processed.
 *
 * If the document is in the error state, this method also does not continue polling, but the extractions won't be
 * available.
 *
 * To avoid flooding the network, there is a pause of at least the number of seconds that is set in the
 * `pollingInterval` property of this class.
 *
 * @warning             This method returns a `BFTask*` resolving to a `GINIDocument` instance representing the
 *                      document. Please notice that the task's result will not be the same document object as the given
 *                      document instance and the given document instance will not be updated with the polled results!
 *
 * @param document      The document that will be polled.
 */
- (BFTask *)pollDocument:(GINIDocument *)document;

/**
* Continually checks the document status until the document is fully processed.
*
* If the document is in the error state, this method also does not continue polling, but the extractions won't be
* available.
*
* To avoid flooding the network, there is a pause of at least the number of seconds that is set in the
* `pollingInterval` property of this class.
*
* @param documentId     The unique identifier of the document which will be polled.
*/
- (BFTask *)pollDocumentWithId:(NSString *)documentId;

/**
 * Gets the preview image for the given page of the given document.
 *
 * @param page          The page number of the document (starting from 1, not 0!).
 * @param document      The document.
 * @param size          The size in which the document will be rendered. Please notice that this is the maximum size,
 *                      meaning that the images dimensions will not exceeding this limit but the rendered image can
 *                      actually have a little smaller dimensions.
 */
- (BFTask *)getPreviewForPage:(NSUInteger)page ofDocument:(GINIDocument *)document withSize:(GiniApiPreviewSize)size;

/**
 * Gets the extractions for the given document.
 *
 * @param document      The document.
 */
- (BFTask *)getExtractionsForDocument:(GINIDocument *)document;

/**
* Gets the extractions for the specific document, including the incubation extractions (see
* http://developer.gini.net/gini-api/html/incubator.html for details on incubating extractions).
*
* @param document       The document.
*/
- (BFTask *)getIncubatorExtractionsForDocument:(GINIDocument *)document;

/**
 * Saves the new values for the given extraction of the given document.
 *
 * Please note that updating an extraction is called "Submitting feedback" in the Gini API documentation.
 *
 * @param extraction    The extraction.
 * @param document      The document.
 */
- (BFTask *)updateExtraction:(GINIExtraction *)extraction forDocument:(GINIDocument *)document;

/**
 * Gets the layout for the given document.
 *
 * @param document      The document.
 */
- (BFTask *)getLayoutForDocument:(GINIDocument *)document;

/**
 * Report an error for a specific document. If the processing result for a document was not satisfactory (e.g.
 * extractions where empty or incorrect), you can create an error report for a document. This allows Gini to analyze and
 * correct the problem that was found. The returned errorId can be used to refer to the reported error towards the Gini
 * support.
 *
 * @warning The owner of this document must agree that Gini can use this document for debugging and error analysis.
 *
 * @param document          The document for which the error is reported.
 * @param summary           A summary for the error (optional).
 * @param description       A detailed description for the error (optional).
 */
- (BFTask *)errorReportForDocument:(GINIDocument *)document summary:(NSString *)summary description:(NSString *)description;
@end
