#ifndef KP_TAB_WIDGET_H
#define KP_TAB_WIDGET_H

#include <QTabWidget>
#include <QList>

class kpDocumentTab;
class kpDocument;
class kpMainWindow;
class kpViewScrollableContainer;

class kpTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit kpTabWidget(kpMainWindow *mainWindow, QWidget *parent = nullptr);
    ~kpTabWidget() override;

    int addTab(kpDocumentTab *docTab);
    void insertTab(int index, kpDocumentTab *docTab);
    void removeTab(int index, kpDocument *deletingDoc = nullptr);
    int currentTabIndex() const;
    void setCurrentTab(int index);

    kpDocumentTab *currentDocumentTab() const;
    kpDocumentTab *documentTabAt(int index) const;
    kpDocument *currentDocument() const;
    int tabCount() const { return m_tabs.count(); }

    int findTabForDocument(kpDocument *doc) const;

Q_SIGNALS:
    void currentTabChanged(int index, kpDocument *deletingDoc = nullptr);
    void tabCloseRequested(int index);
    void tabTitleChanged(int index);

public Q_SLOTS:
    void updateTabTitle(int index);
    void updateCurrentTabTitle();

private Q_SLOTS:
    void onTabChanged(int index);
    void onTabBarCloseRequested(int index);

private:
    void setupTabBar();

    kpMainWindow *m_mainWindow;
    QList<kpDocumentTab *> m_tabs;
    int m_previousIndex;
};

#endif
