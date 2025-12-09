#include "kpDocumentTab.h"
#include "document/kpDocument.h"
#include "kpViewScrollableContainer.h"
#include "views/kpZoomedView.h"
#include "views/manager/kpViewManager.h"
#include "kpThumbnail.h"
#include "commands/kpCommand.h"
#include <QFileInfo>

kpDocumentTab::kpDocumentTab(kpDocument *doc,
                             kpViewScrollableContainer *scrollView,
                             kpZoomedView *mainView,
                             kpViewManager *viewManager,
                             kpThumbnail *thumbnail)
    : m_document(doc)
    , m_scrollView(scrollView)
    , m_mainView(mainView)
    , m_viewManager(viewManager)
    , m_thumbnail(thumbnail)
    , m_documentRestoredPosition(0)
    , m_zoomLevel(100)
    , m_deleteDocument(true)
{
}

kpDocumentTab::~kpDocumentTab()
{
    qDeleteAll(m_undoCommandList);
    m_undoCommandList.clear();
    qDeleteAll(m_redoCommandList);
    m_redoCommandList.clear();
    
    if (m_viewManager) {
        m_viewManager->unregisterAllViews();
        delete m_viewManager;
        m_viewManager = nullptr;
    }
    
    m_mainView = nullptr;
    m_thumbnail = nullptr;
    m_scrollView = nullptr;
    if (m_deleteDocument) {
        delete m_document;
    }
    m_document = nullptr;
}

QString kpDocumentTab::title() const
{
    if (!m_document) {
        return QStringLiteral("Untitled");
    }

    QString name;
    QUrl url = m_document->url();
    
    if (url.isEmpty()) {
        name = m_document->prettyFilename();
    } else {
        name = QFileInfo(url.path()).fileName();
    }

    if (m_document->isModified()) {
        name = name + QStringLiteral(" *");
    }

    return name;
}
