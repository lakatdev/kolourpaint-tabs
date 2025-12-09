#ifndef KP_DOCUMENT_TAB_H
#define KP_DOCUMENT_TAB_H

#include <QWidget>
#include <QList>

class kpDocument;
class kpViewManager;
class kpViewScrollableContainer;
class kpZoomedView;
class kpThumbnail;
class kpCommand;

/**
 * Document tab
 */
class kpDocumentTab
{
public:
    kpDocumentTab(kpDocument *doc, 
                  kpViewScrollableContainer *scrollView,
                  kpZoomedView *mainView,
                  kpViewManager *viewManager,
                  kpThumbnail *thumbnail);
    ~kpDocumentTab();

    kpDocument *document() const { return m_document; }
    void setDocument(kpDocument *doc) { m_document = doc; }
    kpViewScrollableContainer *scrollView() const { return m_scrollView; }
    kpZoomedView *mainView() const { return m_mainView; }
    kpViewManager *viewManager() const { return m_viewManager; }
    kpThumbnail *thumbnail() const { return m_thumbnail; }

    QList<kpCommand *> &undoCommandList() { return m_undoCommandList; }
    QList<kpCommand *> &redoCommandList() { return m_redoCommandList; }
    int documentRestoredPosition() const { return m_documentRestoredPosition; }
    void setDocumentRestoredPosition(int pos) { m_documentRestoredPosition = pos; }
    
    int zoomLevel() const { return m_zoomLevel; }
    void setZoomLevel(int zoom) { m_zoomLevel = zoom; }
    
    void setDeleteDocument(bool deleteDoc) { m_deleteDocument = deleteDoc; }

    QString title() const;

private:
    kpDocument *m_document;
    kpViewScrollableContainer *m_scrollView;
    kpZoomedView *m_mainView;
    kpViewManager *m_viewManager;
    kpThumbnail *m_thumbnail;
    
    QList<kpCommand *> m_undoCommandList;
    QList<kpCommand *> m_redoCommandList;
    int m_documentRestoredPosition;
    
    int m_zoomLevel;
    bool m_deleteDocument;
};

#endif
