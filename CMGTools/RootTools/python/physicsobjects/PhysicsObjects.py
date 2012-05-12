import math

from CMGTools.RootTools.physicsobjects.TauDecayModes import tauDecayModes

class PhysicsObject(object):
    '''Extends the cmg::PhysicsObject functionalities.'''

    def __init__(self, physObj):
        self.physObj = physObj

    def scaleEnergy( self, scale ):
        p4 = self.physObj.p4()
        p4 *= scale 
        self.physObj.setP4( p4 )  
        
    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.physObj, name)

    def __str__(self):
        tmp = '{className} : {pdgId:>3}, pt = {pt:5.1f}, eta = {eta:5.2f}, phi = {phi:5.2f}'
        return tmp.format( className = self.__class__.__name__,
                           pdgId = self.pdgId(),
                           pt = self.pt(),
                           eta = self.eta(),
                           phi = self.phi() )


class Jet( PhysicsObject):
    pass


class GenJet( PhysicsObject):
    pass


class Lepton( PhysicsObject):
    def sip3D(self):
        patLepton = self.physObj.sourcePtr()
        return abs( patLepton.dB(2) ) / patLepton.edB(2) 

    def absIsoFromEA(self,rho,effectiveArea1 = None,effectiveArea2 = None):

        ea1 = rho
        ea2 = rho
        if effectiveArea1 is not None:
            for element in effectiveArea1:
                if abs(self.eta())>= element['etaMin'] and \
                   abs(self.eta())< element['etaMax']:
                    ea1 = ea1 * element['area']
                    break
        else:
            return self.chargedHadronIso()+max(0.,self.photonIso()+self.neutralHadronIso()-ea1)

        if effectiveArea2 is not None:
            for element in effectiveArea2:
                if abs(self.eta())>= element['etaMin'] and \
                   abs(self.eta())< element['etaMax']:
                    ea2 = ea2 * element['area']
            return self.chargedHadronIso()+max(0.,self.photonIso()-ea1)+max(0.,self.neutralHadronIso()-ea2)
        else:
            return self.chargedHadronIso()+max(0.,self.photonIso()+self.neutralHadronIso()-ea1)
                


    def relEffAreaIso(self,rho):
        return 0

    def relEffAreaIso(self,rho):
        return self.absEffAreaIso(rho)/self.pt()


class Muon( Lepton ):

    def looseId( self ):
        '''Loose ID as recommended by mu POG.'''
        return self.isPFMuon() and ( self.isGlobalMuon() or self.isTrackerMuon() )

    def tightId( self ):
        '''Tight ID as recommended by mu POG.'''
        return None

    def mvaIso( self ):
        return self.sourcePtr().userFloat('mvaIsoRings')
    
    def detIso( self, rho ):
        '''Rho corrected detector-based isolation, for the H->ZZ->4l baseline analysis'''
        patMuon = self.sourcePtr() 
        isoEcal = patMuon.ecalIso()
        isoHcal = patMuon.hcalIso()
        isoTk   = patMuon.userIsolation( 7 )
        isoEcal, isoHcal = self.rhoCorrMu(rho, isoEcal, isoHcal)
        return (isoEcal + isoHcal + isoTk)/patMuon.pt()

    def rhoCorrMu(self, rho, ecalIso, hcalIso):
        '''rho correction for the ecal and hcal iso. returns the corrected pair'''
        AreaEcal = [0.074, 0.045] # barrel/endcap 
        AreaHcal = [0.022 , 0.030] # barrel/endcap
        ifid = 1 
        if abs( self.eta() ) < 1.479:
            ifid = 0 # selecting barrel settings
        ecalIso = ecalIso - AreaEcal[ifid] * rho
        hcalIso = hcalIso - AreaHcal[ifid] * rho
        return ecalIso, hcalIso

    def absEffAreaIso(self,rho,effectiveAreas):
        return self.absIsoFromEA(rho,effectiveAreas.muon)


        
class Electron( Lepton ):
    def absEffAreaIso(self,rho,effectiveAreas):
        return self.absIsoFromEA(rho,effectiveAreas.eGamma)



class GenParticle( PhysicsObject):
    def __str__(self):
        base = super(GenParticle, self).__str__()
        theStr = '{base}, status = {status:>2}'.format(base=base, status=self.status())
        return theStr

class GenLepton( GenParticle ):
    def sip3D(self):
        '''Just to make generic code work on GenParticles'''
        return 0
    def relIso(self, dummy):
        '''Just to make generic code work on GenParticles'''
        return 0

    def absIso(self, dummy):
        '''Just to make generic code work on GenParticles'''
        return 0

    def absEffAreaIso(self,rho):
        '''Just to make generic code work on GenParticles'''
        return 0

    def relEffAreaIso(self,rho):
        '''Just to make generic code work on GenParticles'''
        return 0

    
class Tau( Lepton ):
    
    def __init__(self, tau):
        self.tau = tau
        super(Tau, self).__init__(tau)
        self.eOverP = None
        
    def calcEOverP(self):
        if self.eOverP is not None:
            return self.eOverP
        self.leadChargedEnergy = self.tau.leadChargedHadrEcalEnergy() \
                                 + self.tau.leadChargedHadrHcalEnergy()
        # self.leadChargedMomentum = self.tau.leadChargedHadrPt() / math.sin(self.tau.theta())
        self.leadChargedMomentum = self.tau.leadPFChargedHadrCand().energy()
        self.eOverP = self.leadChargedEnergy / self.leadChargedMomentum
        return self.eOverP         

    def __str__(self):
        lep = super(Tau, self).__str__()
        spec = '\tTau: decay = {decMode:<15}, eOverP = {eOverP:4.2f}'.format(
            decMode = tauDecayModes.intToName( self.decayMode() ),
            eOverP = self.calcEOverP()
            )
        return '\n'.join([lep, spec])






def isTau(leg):
    '''Duck-typing a tau'''
    try:
        leg.leadChargedHadrPt()
    except AttributeError:
        return False
    return True

