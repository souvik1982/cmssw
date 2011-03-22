import FWCore.ParameterSet.Config as cms
import os

def fillFromCastor(dir):
    os.system('rfdir ' + dir + ' | awk \'{print $9}\' > /tmp/castordump')
    inFile = open('/tmp/castordump', 'r')
    localdataset=cms.untracked.vstring()
    for line in inFile.readlines():
        if(len(line)==0) : continue
        if(line.find('histograms')>0 or line.find('monitor')>0): continue
        sline=str('rfio://' + dir + '/' + line.split()[0])
        localdataset.extend( [ sline ] )
    os.system('rm /tmp/castordump')
    return localdataset

GluGluToHToZZTo2L2NuM400 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo2L2Nu_M-400')


"""
defines a local source from a given tag
"""
def defineLocalSourceFor( theLocalSrc='GluGluToH200ToZZTo2L2Nu' ) :
    baseCastorDir='/castor/cern.ch/cms/store/cmst3/user/psilva/'
    return fillFromCastor(baseCastorDir + '/' + theLocalSrc)

"""
selects partial data from the all available
"""
def getLocalSourceFor( theLocalSrc='GluGluToH200ToZZTo2L2Nu', ffile=0, fstep=-1  ) :
    localSrc = defineLocalSourceFor( theLocalSrc )
    prunedLocalSrc = cms.untracked.vstring()
    if(fstep<0): fstep = len(localSrc)
    lfile=ffile+fstep
    if(ffile<lfile) :
        fctr=0
        for i in localSrc :
            if(fctr>=ffile and fctr<lfile) :
                prunedLocalSrc.append( i )
            fctr+=1
    return prunedLocalSrc
