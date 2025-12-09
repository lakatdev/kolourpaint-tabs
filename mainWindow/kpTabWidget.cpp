#include "kpTabWidget.h"
#include "kpDocumentTab.h"
#include "kpMainWindow.h"
#include "document/kpDocument.h"
#include "kpViewScrollableContainer.h"

#include <QTabBar>

kpTabWidget::kpTabWidget(kpMainWindow *mainWindow, QWidget *parent)
    : QTabWidget(parent)
    , m_mainWindow(mainWindow)
    , m_previousIndex(-1)
{
    setupTabBar();
    
    connect(this, &QTabWidget::currentChanged, 
            this, &kpTabWidget::onTabChanged);
}

kpTabWidget::~kpTabWidget()
{
    qDeleteAll(m_tabs);
}

void kpTabWidget::setupTabBar()
{
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);
    setElideMode(Qt::ElideRight);
    
    connect(tabBar(), &QTabBar::tabCloseRequested,
            this, &kpTabWidget::onTabBarCloseRequested);
}

int kpTabWidget::addTab(kpDocumentTab *docTab)
{
    if (!docTab) {
        return -1;
    }

    m_tabs.append(docTab);
    
    int index = QTabWidget::addTab(docTab->scrollView(), docTab->title());
    
    if (m_previousIndex < 0) {
        m_previousIndex = 0;
    }
    
    if (docTab->document()) {
        connect(docTab->document(), &kpDocument::documentSaved,
                this, &kpTabWidget::updateCurrentTabTitle);
        connect(docTab->document(), &kpDocument::documentModified,
                this, &kpTabWidget::updateCurrentTabTitle);
    }
    
    return index;
}

void kpTabWidget::insertTab(int index, kpDocumentTab *docTab)
{
    if (!docTab) {
        return;
    }

    m_tabs.insert(index, docTab);
    
    QTabWidget::insertTab(index, docTab->scrollView(), docTab->title());
    
    if (m_previousIndex < 0) {
        m_previousIndex = 0;
    }
    
    if (docTab->document()) {
        connect(docTab->document(), &kpDocument::documentSaved,
                this, &kpTabWidget::updateCurrentTabTitle);
        connect(docTab->document(), &kpDocument::documentModified,
                this, &kpTabWidget::updateCurrentTabTitle);
    }
}

void kpTabWidget::removeTab(int index, kpDocument *deletingDoc)
{
    if (index < 0 || index >= m_tabs.count()) {
        return;
    }

    int newCurrentIndex = -1;
    if (m_tabs.count() > 1) {
        if (index == currentIndex()) {
            newCurrentIndex = (index < m_tabs.count() - 1) ? index : index - 1;
        }
        else {
            int current = currentIndex();
            newCurrentIndex = (current > index) ? current - 1 : current;
        }
    }

    disconnect(this, &QTabWidget::currentChanged, this, &kpTabWidget::onTabChanged);
    disconnect(tabBar(), &QTabBar::tabCloseRequested, this, &kpTabWidget::onTabBarCloseRequested);
    
    kpDocumentTab *docTab = m_tabs.takeAt(index);
    QTabWidget::removeTab(index);
    
    connect(this, &QTabWidget::currentChanged, this, &kpTabWidget::onTabChanged);
    connect(tabBar(), &QTabBar::tabCloseRequested, this, &kpTabWidget::onTabBarCloseRequested);
    
    if (newCurrentIndex >= 0 && newCurrentIndex < m_tabs.count()) {
        Q_EMIT currentTabChanged(newCurrentIndex, deletingDoc);
    }
    
    delete docTab;
}

int kpTabWidget::currentTabIndex() const
{
    return currentIndex();
}

void kpTabWidget::setCurrentTab(int index)
{
    if (index >= 0 && index < m_tabs.count()) {
        setCurrentIndex(index);
    }
}

kpDocumentTab *kpTabWidget::currentDocumentTab() const
{
    int index = currentIndex();
    if (index >= 0 && index < m_tabs.count()) {
        return m_tabs.at(index);
    }
    return nullptr;
}

kpDocumentTab *kpTabWidget::documentTabAt(int index) const
{
    if (index >= 0 && index < m_tabs.count()) {
        return m_tabs.at(index);
    }
    return nullptr;
}

kpDocument *kpTabWidget::currentDocument() const
{
    kpDocumentTab *tab = currentDocumentTab();
    return tab ? tab->document() : nullptr;
}

int kpTabWidget::findTabForDocument(kpDocument *doc) const
{
    for (int i = 0; i < m_tabs.count(); ++i) {
        if (m_tabs.at(i)->document() == doc) {
            return i;
        }
    }
    return -1;
}

void kpTabWidget::updateTabTitle(int index)
{
    if (index >= 0 && index < m_tabs.count()) {
        kpDocumentTab *tab = m_tabs.at(index);
        setTabText(index, tab->title());
        Q_EMIT tabTitleChanged(index);
    }
}

void kpTabWidget::updateCurrentTabTitle()
{
    updateTabTitle(currentIndex());
}

void kpTabWidget::onTabChanged(int index)
{
    Q_EMIT currentTabChanged(index, nullptr);
    m_previousIndex = index;
}

void kpTabWidget::onTabBarCloseRequested(int index)
{
    Q_EMIT tabCloseRequested(index);
}
