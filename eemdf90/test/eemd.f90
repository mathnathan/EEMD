      subroutine eemd(LXY,indata,An,Nesb,Nm,idum,rslt)
!-----------------------------------------------------------------------------
! This is a subroutine to decompose data(LXY) in terms of its EEMD component.
! When the added noise amplitude is zero (An=0) and the ensemble number
! is one (Nesb=0), the code degenerates to standard EMD
!
! In this code, the number of the oscillatory components is specified as an
! input, Nm. For most cases, automatic calculation, Nm can be set to log2(LXY)-2
!
! INPUT DATA:
!		indata(LXY)		input array with a length of LXY
!  		An 			amplitude of added noise
!  		Nesb 			number of ensemble members
!  		Nm 			number of mode in each decomposition
!  		idum 			seed for the random number
! OUTPUT DATA:
!  		rslt(LXY, Nm+2) 	output rseults, which contains Nm+2 columns
!					The first column is the origninal indata;
!					The second column is the first component;
!					The third column is the second component;
!					and so on
!					The last column is the residue
!-------------------------------------------------------------------------------
      implicit none
      INCLUDE 'eemd.common'
      real :: An				! noise amplitude
      integer, intent(in) :: LXY, Nesb, Nm
      integer, intent(inout):: idum
      real, dimension(LXY), intent(in) :: indata
      real, dimension(LXY) :: ximf		! data for sifting
      real, dimension(LXY) :: spmax		! upper envelope, cubic spline
      real, dimension(LXY) :: spmin		! lower envelope, cubic spline
      real, dimension(LXY) :: ave		! the average of spmax and spmin
      real, dimension(LXY) :: remp		! the input data for sifting
      real, dimension(LXY) :: rem		! the remainder (remp-ximf)

      real, dimension(LXY,Nm+2) :: rslt		! the final output data
      real, dimension(LXY,Nm+2) :: allmode	! the results of a single EMD decomposition

      integer :: nmax, nmin			! numbers of maximum and minimum
      real, dimension(LXY) :: trend		! linear trend of indata
      real :: std		! standard deviation of the linearly detrended indata
      integer:: i,j,IE,im,ii
      real :: fNesb, gasdev


!  initialize the output
      rslt=0.0

!
!  ensemble EMD
!
!  *******************************************************
      do IE=1,Nesb
!  *******************************************************
!
        call standev(LXY,indata,trend, std)

!  inputted data + noise
        if(Nesb.eq.1) then
          ximf=indata
        else
          do i=1,LXY
            ximf(i)=indata(i)+An*std*gasdev(idum)
          enddo
        endif

!  save modified data
        do i=1,LXY
          allmode(i,1)=ximf(i)
        enddo

!  calculate modes
!       =======================================================
        do im=1,Nm
!       =======================================================
!
!  leave a copy of the input data before IMF is calculated
          remp=ximf
!
!  Sifting
!         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
          do ii=1,10
!         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

            call min_max(LXY,ximf,spmax,spmin,nmax,nmin)
            call natural_spline(spmax,LXY,nmax)
            call natural_spline(spmin,LXY,nmin)

            ave=(spmax+spmin)/2.0
            ximf=ximf-ave

!         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
          enddo
!         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     

          rem=remp-ximf

          do i=1,LXY
            allmode(i,im+1)=ximf(i)
          enddo

          ximf=rem

!       =======================================================
        enddo
!       =======================================================

        do i=1,LXY
          allmode(i,Nm+2)=ximf(i)
        enddo

        do j=1,Nm+2
          do i=1,LXY
            rslt(i,j)=rslt(i,j)+allmode(i,j)
          enddo
        enddo

!     ---------------------------------------------------------
      enddo
!     ---------------------------------------------------------
      
      fNesb=real(Nesb)
      rslt=rslt/fNesb

      end subroutine eemd

