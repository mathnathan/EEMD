      subroutine standev(nsize, indata, trend, std)
!-------------------------------------------------------------------
!  This is a program to obtain standard deviation of the linearly
!  detrended data
!  
!  PARAMETERS:
!  nsize    :    indata size
!  indata   :    input data
!  trend    :    the linear trend of "indata"
!  std      :    standard deviation
!--------------------------------------------------------------------

      implicit none

      integer, intent(in) :: nsize
      real, dimension(nsize) :: indata
      real, dimension(nsize), intent(out):: trend
      real, intent(out) :: std
      real sigmaX, sigmaY, sigmaX2, sigmaXY, real_nsize, Xbar, Ybar, real_i
      integer :: i
      real :: trend_const, trend_slope, temp

      sigmaX = 0.0
      sigmaY = 0.0
      do i = 1, nsize
        sigmaX = sigmaX + real(i)
        sigmaY = sigmaY + indata(i)
      enddo

      real_nsize=real(nsize)

      Xbar=sigmaX/real_nsize
      Ybar=sigmaY/real_nsize

      sigmaX2 = 0.0
      sigmaXY = 0.0
      do i = 1, nsize
        real_i=real(i)
        sigmaX2 = sigmaX2+(real_i-xbar)*(real_i-xbar)
        sigmaXY = sigmaXY+(real_i-xbar)*(indata(i)-ybar)
      enddo

      trend_slope=sigmaXY/sigmaX2
      trend_const=Ybar-trend_slope*Xbar

      do i=1, nsize
        trend(i)= trend_const + trend_slope*real(i)
      enddo

      std=0.0

      do i=1,nsize
        temp= indata(i)-trend(i)
        std=std+temp*temp
      enddo
      std=std/real(nsize)
      std=sqrt(std)

      end subroutine standev

