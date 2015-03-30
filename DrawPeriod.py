import sqlite3
import datetime
import string
import time
import plotly.plotly as py
from plotly.graph_objs import *

def PlotRT(sqlc,plotname):
  TimeX=[]
  Y0  =[]
  Y1  =[] 

  CountRow = 0
  for row in sqlc:
    TimeStamp = row[0]
    DS        = float(row[1])
    Humi      = float(row[2])
    TimeX.append(TimeStamp)
    Y0.append(DS)
    Y1.append(Humi)
    #print TimeStamp,DS,Humi
    #CountRow+=1
  
  #print CountRow

  timestr = time.strftime("%H:%M:%S %d %b %Y", time.localtime())

  trace0 = Scatter(
    x=TimeX,
    y=Y0,
    name='RT',
    line=Line(shape='spline')
  )

  trace1 = Scatter(
    x=TimeX,
    y=Y1,
    name='Humidity',
    yaxis='y2',
    line=Line(shape='spline')
  )

  layout = Layout(
    title=plotname, 
    xaxis=XAxis(title='Date Time'),
    yaxis=YAxis(title='Temperature',range=[16,30]),
    #yaxis2=YAxis(title='Humidity',range=[45,80],overlaying='y',side='right'),
    yaxis2=YAxis(title='Humidity',overlaying='y',side='right'),
    annotations=Annotations([
          Annotation(
            x=1,
            y=0.40,
            xref='paper',
            yref='paper',
            text= 'Last Update ' + timestr,
            showarrow=False,
            )
      ]),
    )

  data = Data([trace0,trace1])
  fig = Figure(data=data,layout=layout)
  unique_url = py.plot(fig, filename = plotname)

conn = sqlite3.connect('/home/pi/TempMod/HomeLogger.db')
c = conn.cursor()

c.execute("select TimeStamp,AVG(AM_Room),AVG(AM_Humi) from HomeLogger where strftime('%s',TimeStamp) > strftime('%s', 'now','localtime','-180 minutes') group by strftime('%s',TimeStamp)/180");
PlotRT(c,"Home-Last 3 Hours")

c.execute("select TimeStamp,AVG(AM_Room),AVG(AM_Humi) from HomeLogger where strftime('%s',TimeStamp) > strftime('%s', 'now','localtime','-720 minutes') group by strftime('%s',TimeStamp)/(4*180)")
PlotRT(c,"Home-Last 12 Hours")

c.execute("select TimeStamp,AVG(AM_Room),AVG(AM_Humi) from HomeLogger where strftime('%s',TimeStamp) > strftime('%s', 'now','localtime','-1440 minutes') group by strftime('%s',TimeStamp)/(2*4*180)")
PlotRT(c,"Home-Last 24 Hours")

c.execute("select max(strftime('%s',TimeStamp))-min(strftime('%s',TimeStamp)) from HomeLogger;")
CountInterval = int(c.fetchone()[0])/250

c.execute("select TimeStamp,AVG(AM_Room),AVG(AM_Humi) from HomeLogger group by strftime('%%s',TimeStamp)/%d" % CountInterval)
PlotRT(c,"Home-All")

c.close()