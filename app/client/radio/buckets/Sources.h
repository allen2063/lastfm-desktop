/***************************************************************************
 *   Copyright 2005-2008 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/
 
#ifndef SOURCES_H
#define SOURCES_H

#include <QMainWindow>
#include "SeedTypes.h"

namespace Unicorn{ class TabWidget; }

class Sources : public QWidget
{
	Q_OBJECT
public:
	Sources();
    
    void setupUi();

	struct {
		class Unicorn::TabWidget* tabWidget;
        class PrimaryListView* stationsBucket;
		class PrimaryListView* friendsBucket;
		class PrimaryListView* tagsBucket;
        class RadioControls* controls;
        class QLineEdit* freeInput;
        class QComboBox* inputSelector;
	} ui;
    
    void connectToAmp( class Amp* amp );
    
    QSize sizeHint() const
    {
        return QSize( 0, 200 );
    }
    
protected:
    class WsAccessManager* m_accessManager;
    class Amp* m_connectedAmp;
	
protected slots:
    void onAmpSeedRemoved( QString, Seed::Type );
    void onUserGetFriendsReturn( class WsReply* );
    void onUserGetTopTagsReturn( class WsReply* );
    void onUserGetPlaylistsReturn( class WsReply* );

    void onAuthUserInfoReturn( class WsReply* );
    void authUserIconDataDownloaded();
    
    void onItemDoubleClicked( const class QModelIndex& index );
    void onDnDAnimationFinished();
    void onFreeInputReturn();
    
    void onAmpDestroyed(){ m_connectedAmp = 0; }

};


#include <QMouseEvent>
#include <QModelIndex>
#include <QListWidget>
#include "PlayableListItem.h"
#include "PlayableMimeData.h"
#include <QDebug>

class PrimaryListView : public QListWidget
{
    Q_OBJECT
    
    friend class Sources;
public:
	PrimaryListView( QWidget* parent ): QListWidget( parent )
    {
        setViewMode( QListView::IconMode );
        setFlow( QListView::LeftToRight );
        setResizeMode( QListView::Adjust );
        setSpacing( 0 );
        setUniformItemSizes( true );
    }
    
protected:
    QMimeData* mimeData( const QList<QListWidgetItem *> items ) const
    {
        if( items.isEmpty() )
            return 0;

        PlayableListItem* item = dynamic_cast<PlayableListItem*>( items.first() );
        
        PlayableMimeData* data = new PlayableMimeData();
        data->setType( item->playableType() );
        data->setText( item->text() );
        data->setImageData( item->icon().pixmap(34).toImage() );
        
        if( item->playableType() == Seed::PreDefinedType )
            data->setRQL( item->rql() );
        return data;
    }
    
    void focusOutEvent( QFocusEvent* event )
    {
        clearSelection();
        QListWidget::focusOutEvent( event );
    }
	
};

#endif //SOURCES_H