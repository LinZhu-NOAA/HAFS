// This file was automatically generated by 'make' from file 'fly_SH_to_spat.gen.c'.
// To modify it, please consider modifying fly_SH_to_spat.gen.c
/*
 * Copyright (c) 2010-2019 Centre National de la Recherche Scientifique.
 * written by Nathanael Schaeffer (CNRS, ISTerre, Grenoble, France).
 * 
 * nathanael.schaeffer@univ-grenoble-alpes.fr
 * 
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software. You can use,
 * modify and/or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 * 
 */
 

  #ifndef SHT_GRAD
	static void GEN3(SHsphtor_to_spat_fly,NWAY,SUFFIX)(shtns_cfg shtns, cplx *Slm, cplx *Tlm, double *Vt, double *Vp, const long int llim) {
  #else
	static void GEN3(SHtor_to_spat_fly,NWAY,SUFFIX)(shtns_cfg shtns, cplx *Tlm, double *Vt, double *Vp, const long int llim) {
  #endif

	#if !defined( _GCC_VEC_ ) && (NWAY & 1)
	#error "NWAY must be even when compiled without explicit vectorization."
	#endif

  #ifndef SHT_AXISYM
	v2d *BtF, *BpF;
  #ifndef SHTNS_ISHIOKA
  #else
  #endif
	#define vr(l) vall( ((double*) VWl)[4*(l)]   )
	#define vi(l) vall( ((double*) VWl)[4*(l)+1] )
	#define wr(l) vall( ((double*) VWl)[4*(l)+2] )
	#define wi(l) vall( ((double*) VWl)[4*(l)+3] )
	unsigned im, imlim;
  #else
	double* const BpF = Vp;
  #endif
	long int nk, k,l,m;
	double *alm, *al;
	s2d *ct, *st;
	int robert_form;
	v2d VWl[llim*2+4];
  #ifdef SHTNS_ISHIOKA
  #endif

  #ifndef SHT_AXISYM
	BtF = (v2d*) Vt;	BpF = (v2d*) Vp;
	if (shtns->fftc_mode > 0) {		// alloc memory for the FFT
		unsigned long nv = shtns->nspat;
		BtF = (v2d*) VMALLOC( 2*nv * sizeof(double) );
		BpF = BtF + nv/2;
	}
	  #ifdef SHT_GRAD
		k=0; do { BtF[k]=vdup(0.0); } while(++k<NLAT_2);
	  #endif
	imlim = MTR;
	#ifdef SHT_VAR_LTR
		if (imlim*MRES > (unsigned) llim) imlim = ((unsigned) llim)/MRES;		// 32bit mul and div should be faster
	#endif
  #else
	#ifdef SHT_GRAD
		if (Vt != NULL) { k=0; do { ((v2d*)Vt)[k]=vdup(0.0); } while(++k<NLAT_2); }
	#endif
  #endif
	robert_form = shtns->robert_form;

	ct = (s2d*) shtns->ct;		st = (s2d*) shtns->st;
	{	//	im=0;
		alm = shtns->alm;
		double* const Tl0 = (double*) VWl + llim+2;
 		l=1;
		do {		// for m=0, compress the complex Q,S,T to double
			Tl0[l-1] = (double) Tlm[l];	//	Tl[l] = (double) Tlm[l+1];
			++l;
		} while(l<=llim);
		k=0;	nk = NLAT_2;
		nk = ((unsigned)(nk+VSIZE2-1)) / VSIZE2;
		do {
			l=0;	al = alm;
			rnd cost[NWAY], y0[NWAY], y1[NWAY];
			rnd sint[NWAY], dy0[NWAY], dy1[NWAY];
			rnd pe[NWAY], po[NWAY];
			for (int j=0; j<NWAY; ++j) {
				cost[j] = vread(ct, j+k);
				sint[j] = -vread(st, j+k);
				y0[j] = vall(al[0]);
				dy0[j] = vall(0.0);
				po[j] = dy0[j];
			}
			if (robert_form) {
				for (int j=0; j<NWAY; ++j) sint[j] *= -sint[j];
			}
			for (int j=0; j<NWAY; ++j) {
				y1[j]  = vall(al[0]*al[1]) * cost[j];
				dy1[j] = vall(al[0]*al[1]) * sint[j];
			}
			for (int j=0; j<NWAY; ++j) {
				pe[j] = -dy1[j] * vall(Tl0[0]);
			}
			al+=2;	l+=2;
			while(l<llim) {
				for (int j=0; j<NWAY; ++j) {
					dy0[j] = vall(al[1])*(cost[j]*dy1[j] + y1[j]*sint[j]) + vall(al[0])*dy0[j];
					y0[j]  = vall(al[1])*(cost[j]*y1[j]) + vall(al[0])*y0[j];
				}
				for (int j=0; j<NWAY; ++j) {
					po[j] -= dy0[j] * vall(Tl0[l-1]);
				}
				for (int j=0; j<NWAY; ++j) {
					dy1[j] = vall(al[3])*(cost[j]*dy0[j] + y0[j]*sint[j]) + vall(al[2])*dy1[j];
					y1[j]  = vall(al[3])*(cost[j]*y0[j]) + vall(al[2])*y1[j];
				}
				for (int j=0; j<NWAY; ++j) {
					pe[j] -= dy1[j] * vall(Tl0[l]);
				}
				al+=4;	l+=2;
			}
			if (l==llim) {
				for (int j=0; j<NWAY; ++j) {
					dy0[j] = vall(al[1])*(cost[j]*dy1[j] + y1[j]*sint[j]) + vall(al[0])*dy0[j];
					y0[j]  = vall(al[1])*cost[j]*y1[j] + vall(al[0])*y0[j];
				}
				for (int j=0; j<NWAY; ++j) {
					po[j] -= dy0[j] * vall(Tl0[l-1]);
				}
			}
		#ifndef SHTNS4MAGIC
			for (int j=0; j<NWAY; ++j) {
				S2D_STORE(BpF, j+k, pe[j], po[j])
			}
		#else
			for (int j=0; j<NWAY; ++j) {
				if ((k+j)>=nk) break;
				S2D_STORE_4MAGIC((double*)BpF, j+k, pe[j], po[j]);
			}
		#endif
			k+=NWAY;
		} while (k < nk);
	}

  #ifndef SHT_AXISYM
		BtF += NLAT_2;	BpF += NLAT_2;
	for(im=1; im<=imlim; ++im) {
		m = im*MRES;
		//l = LiM(shtns, 0,im);
		l = (im*(2*(LMAX+1)-(m+MRES)))>>1;
		//alm = shtns->alm[im];
		//alm = shtns->alm[0] + im*(2*LMAX - (im-1)*MRES);        // for m > 0
		alm += 2*(LMAX+1-m+MRES);

  #ifndef SHT_GRAD
		SH_vect_to_2scal(shtns->mx_stdt + 2*l, llim, m, &Slm[l], &Tlm[l], (cplx*) VWl);
  #else
		SHtor_to_2scal(shtns->mx_stdt + 2*l, llim, m, &Tlm[l], (cplx*) VWl);
  #endif

	#ifndef SHTNS_ISHIOKA
	#else
		// pre-processing for recurrence relation of Ishioka
		const double* restrict xlm = shtns->xlm + 3*im*(2*(LMAX+4) -m+MRES)/4;
		SH2_to_ishioka(xlm, VWl+2*m, llim-m+1);
	#endif

		k=0;	l=shtns->tm[im];
		l>>=1;		// stay on a 16 byte boundary
		while (k<l) {	// polar optimization
	#ifndef SHTNS4MAGIC
			BtF[k] = vdup(0.0);				BtF[(NPHI-2*im)*NLAT_2 + k] = vdup(0.0);
			BtF[NLAT_2-l+k] = vdup(0.0);	BtF[(NPHI+1-2*im)*NLAT_2 -l+k] = vdup(0.0);
			BpF[k] = vdup(0.0);				BpF[(NPHI-2*im)*NLAT_2 + k] = vdup(0.0);
			BpF[NLAT_2-l+k] = vdup(0.0);	BpF[(NPHI+1-2*im)*NLAT_2 -l+k] = vdup(0.0);
	#else
			BtF[2*k] = vdup(0.0);			BtF[(NPHI-2*im)*NLAT_2 + 2*k] = vdup(0.0);
			BtF[2*k+1] = vdup(0.0);			BtF[(NPHI-2*im)*NLAT_2 +2*k+1] = vdup(0.0);
			BpF[2*k] = vdup(0.0);			BpF[(NPHI-2*im)*NLAT_2 + 2*k] = vdup(0.0);
			BpF[2*k+1] = vdup(0.0);			BpF[(NPHI-2*im)*NLAT_2 +2*k+1] = vdup(0.0);
	#endif
			++k;
		}
		#ifdef _GCC_VEC_
		k = ((unsigned) k) / (VSIZE2/2);
		#else
		k *= 2;
		#endif
		do {
			al = alm;
			rnd cost[NWAY], y0[NWAY], y1[NWAY];
			rnd ter[NWAY], tei[NWAY], tor[NWAY], toi[NWAY];
			rnd per[NWAY], pei[NWAY], por[NWAY], poi[NWAY];
			for (int j=0; j<NWAY; ++j) {
				cost[j] = vread(st, k+j);
				y0[j] = vall(1.0);
			}
			l=m;
			if (robert_form == 0) l=m-1;
			long int ny = 0;
		  if ((int)llim <= SHT_L_RESCALE_FLY) {
			do {		// sin(theta)^m
				if (l&1) for (int j=0; j<NWAY; ++j) y0[j] *= cost[j];
				for (int j=0; j<NWAY; ++j) cost[j] *= cost[j];
			} while(l >>= 1);
		  } else {
			long int nsint = 0;
			do {		// sin(theta)^m		(use rescaling to avoid underflow)
				if (l&1) {
					for (int j=NWAY-1; j>=0; --j) y0[j] *= cost[j];
					ny += nsint;
					if (vlo(y0[NWAY-1]) < (SHT_ACCURACY+1.0/SHT_SCALE_FACTOR)) {
						ny--;
						for (int j=NWAY-1; j>=0; --j) y0[j] *= vall(SHT_SCALE_FACTOR);
					}
				}
				for (int j=NWAY-1; j>=0; --j) cost[j] *= cost[j];
				nsint += nsint;
				if (vlo(cost[NWAY-1]) < 1.0/SHT_SCALE_FACTOR) {
					nsint--;
					for (int j=NWAY-1; j>=0; --j) cost[j] *= vall(SHT_SCALE_FACTOR);
				}
			} while(l >>= 1);
		  }
			#ifdef SHTNS_ISHIOKA
			al = shtns->clm + im*(2*(LMAX+1) - m+MRES)/2;		// for ishioka recurrence
			#endif
			for (int j=0; j<NWAY; ++j) {
				cost[j] = vread(ct, j+k);
				#ifndef SHTNS_ISHIOKA
				y0[j] *= vall(al[0]);
				#else
				cost[j] *= cost[j];		// cos(theta)^2
				#endif
			}
			for (int j=0; j<NWAY; ++j) {
				#ifndef SHTNS_ISHIOKA
				y1[j]  = (vall(al[1])*y0[j]) *cost[j];		//	y1[j] = vall(al[1])*cost[j]*y0[j];
				#else
				y1[j] = (vall(al[1])*cost[j] + vall(al[0]))*y0[j];
				#endif
				por[j] = vall(0.0);		tei[j] = vall(0.0);
				tor[j] = vall(0.0);		pei[j] = vall(0.0);
				poi[j] = vall(0.0);		ter[j] = vall(0.0);
				toi[j] = vall(0.0);		per[j] = vall(0.0);
			}
			l=m;		al+=2;
			while ((ny<0) && (l<llim)) {		// ylm treated as zero and ignored if ny < 0
				#ifndef SHTNS_ISHIOKA
				for (int j=0; j<NWAY; ++j) {
					y0[j] = (vall(al[1])*cost[j])*y1[j] + vall(al[0])*y0[j];
				}
				for (int j=0; j<NWAY; ++j) {
					y1[j] = (vall(al[3])*cost[j])*y0[j] + vall(al[2])*y1[j];
				}
				al+=4;
				#else
				for (int j=0; j<NWAY; ++j) {
					rnd tmp = y1[j];
					y1[j] = (vall(al[1])*cost[j] + vall(al[0]))*y1[j] + y0[j];
					y0[j] = tmp;
				}
				al+=2;
				#endif
				l+=2;
				if (fabs(vlo(y0[NWAY-1])) > SHT_ACCURACY*SHT_SCALE_FACTOR + 1.0) {		// rescale when value is significant
					++ny;
					for (int j=0; j<NWAY; ++j) {
						y0[j] *= vall(1.0/SHT_SCALE_FACTOR);		y1[j] *= vall(1.0/SHT_SCALE_FACTOR);
					}
				}
			}
		  if (ny == 0) {
		#ifndef SHTNS_ISHIOKA
			while (l<llim) {	// compute even and odd parts
				for (int j=0; j<NWAY; ++j) {	ter[j] += y0[j]  * vr(l);		tei[j] += y0[j] * vi(l);	}
				for (int j=0; j<NWAY; ++j) {	per[j] += y0[j]  * wr(l);		pei[j] += y0[j] * wi(l);	}
				for (int j=0; j<NWAY; ++j) {
					y0[j] = vall(al[1])*(cost[j]*y1[j]) + vall(al[0])*y0[j];
				}
				for (int j=0; j<NWAY; ++j) {	tor[j] += y1[j]  * vr(l+1);		toi[j] += y1[j] * vi(l+1);	}
				for (int j=0; j<NWAY; ++j) {	por[j] += y1[j]  * wr(l+1);		poi[j] += y1[j] * wi(l+1);	}
				for (int j=0; j<NWAY; ++j) {
					y1[j] = vall(al[3])*(cost[j]*y0[j]) + vall(al[2])*y1[j];
				}
				l+=2;	al+=4;
			}
				for (int j=0; j<NWAY; ++j) {	ter[j] += y0[j]  * vr(l);		tei[j] += y0[j] * vi(l);	}
				for (int j=0; j<NWAY; ++j) {	per[j] += y0[j]  * wr(l);		pei[j] += y0[j] * wi(l);	}
			if (l==llim) {
				for (int j=0; j<NWAY; ++j) {	tor[j] += y1[j]  * vr(l+1);		toi[j] += y1[j] * vi(l+1);	}
				for (int j=0; j<NWAY; ++j) {	por[j] += y1[j]  * wr(l+1);		poi[j] += y1[j] * wi(l+1);	}
			}
		#else
			while (l<llim) {	// compute even and odd parts
				for (int j=0; j<NWAY; ++j) {	ter[j] += y0[j]  * vr(l);		tei[j] += y0[j] * vi(l);	}
				for (int j=0; j<NWAY; ++j) {	per[j] += y0[j]  * wr(l);		pei[j] += y0[j] * wi(l);	}
				for (int j=0; j<NWAY; ++j) {	tor[j] += y0[j]  * vr(l+1);		toi[j] += y0[j] * vi(l+1);	}
				for (int j=0; j<NWAY; ++j) {	por[j] += y0[j]  * wr(l+1);		poi[j] += y0[j] * wi(l+1);	}
				for (int j=0; j<NWAY; ++j) {
					rnd tmp = y1[j];
					y1[j] = (vall(al[1])*cost[j] + vall(al[0]))*y1[j] + y0[j];
					y0[j] = tmp;
				}
				l+=2;	al+=2;
			}
				for (int j=0; j<NWAY; ++j) {	ter[j] += y0[j]  * vr(l);		tei[j] += y0[j] * vi(l);	}
				for (int j=0; j<NWAY; ++j) {	per[j] += y0[j]  * wr(l);		pei[j] += y0[j] * wi(l);	}
			if (l==llim) {
				for (int j=0; j<NWAY; ++j) {	tor[j] += y0[j]  * vr(l+1);		toi[j] += y0[j] * vi(l+1);	}
				for (int j=0; j<NWAY; ++j) {	por[j] += y0[j]  * wr(l+1);		poi[j] += y0[j] * wi(l+1);	}
			}
			// correct the odd part:
			for (int j=0; j<NWAY; ++j) cost[j] = vread(ct, k+j);
			for (int j=0; j<NWAY; ++j) {  tor[j] *= cost[j];	toi[j] *= cost[j]; }
			for (int j=0; j<NWAY; ++j) {  por[j] *= cost[j];	poi[j] *= cost[j]; }
		#endif
		  }
		#ifndef SHTNS4MAGIC
		  #ifdef _GCC_VEC_
			for (int j=0; j<NWAY; ++j) {
				S2D_CSTORE(BtF, k+j, ter[j], tor[j], tei[j], toi[j])
				S2D_CSTORE(BpF, k+j, per[j], por[j], pei[j], poi[j])
			}
		  #else
			for (int j=0; j<NWAY/2; ++j) {		// NWAY is even when _GCC_VEC_ is not defined
				S2D_CSTOREX(BtF, k/2+j, 2*j, ter, tor, tei, toi)
				S2D_CSTOREX(BpF, k/2+j, 2*j, per, por, pei, poi)
			}
		  #endif
		#else
			for (int j=0; j<NWAY; ++j) {
				if ((k+j)>=nk) break;
				S2D_CSTORE_4MAGIC((double*) BtF, (double*) (BtF + (NPHI-2*im)*NLAT_2), k+j, ter[j], tor[j], tei[j], toi[j]);
				S2D_CSTORE_4MAGIC((double*) BpF, (double*) (BpF + (NPHI-2*im)*NLAT_2), k+j, per[j], por[j], pei[j], poi[j]);
			}
		#endif
			k+=NWAY;
		} while (k < nk);
		BtF += NLAT_2;	BpF += NLAT_2;
	}

	for (k=0; k < NLAT_2*(NPHI-1-2*imlim); ++k) {	// padding for high m's
		BtF[k] = vdup(0.0);	BpF[k] = vdup(0.0);
	}
	BtF -= NLAT_2*(imlim+1);	BpF -= NLAT_2*(imlim+1);
    // NPHI > 1 as SHT_AXISYM is not defined.
  	if (shtns->fftc_mode >= 0) {
		if (shtns->fftc_mode != 1) {
			fftw_execute_dft(shtns->ifftc, (cplx *) BtF, (cplx *) Vt);
			fftw_execute_dft(shtns->ifftc, (cplx *) BpF, (cplx *) Vp);
		} else {		// split dft
			fftw_execute_split_dft(shtns->ifftc,((double*)BtF)+1, ((double*)BtF), Vt+NPHI, Vt);
			fftw_execute_split_dft(shtns->ifftc,((double*)BpF)+1, ((double*)BpF), Vp+NPHI, Vp);
			VFREE(BtF);		// this frees also BpF.
		}
	}
  #endif

	#undef tr
	#undef ti
  }

  #ifndef SHT_AXISYM

  #ifndef SHT_GRAD
	static void GEN3(SHsphtor_m_to_spat_fly,NWAY,SUFFIX)(shtns_cfg shtns, int im, cplx *Slm, cplx *Tlm, cplx *Vt, cplx *Vp, const long int llim) {
  #else
	static void GEN3(SHtor_m_to_spat_fly,NWAY,SUFFIX)(shtns_cfg shtns, int im, cplx *Tlm, cplx *Vt, cplx *Vp, const long int llim) {
  #endif

	v2d *BtF, *BpF;
	#define vr(l) vall( ((double*) VWl)[4*(l)]   )
	#define vi(l) vall( ((double*) VWl)[4*(l)+1] )
	#define wr(l) vall( ((double*) VWl)[4*(l)+2] )
	#define wi(l) vall( ((double*) VWl)[4*(l)+3] )
	long int nk, k,l,m;
	double *alm, *al;
	s2d *ct, *st;
	int robert_form;

	BtF = (v2d*) Vt;	BpF = (v2d*) Vp;

	nk = NLAT_2;
	#if _GCC_VEC_
		nk = ((unsigned)(nk+VSIZE2-1)) / VSIZE2;
	#endif
	ct = (s2d*) shtns->ct;		st = (s2d*) shtns->st;
	robert_form = shtns->robert_form;

	if (im == 0) {
		double Tl0[llim];

		#ifdef SHT_GRAD
			k=0; do { BtF[k]=vdup(0.0); } while(++k<NLAT);
		#endif

 		l=1;
		alm = shtns->alm;
		do {		// for m=0, compress the complex Q,S,T to double
			Tl0[l-1] = (double) Tlm[l];	//	Tl[l] = (double) Tlm[l+1];
			++l;
		} while(l<=llim);
		k=0;
		do {
			l=0;	al = alm;
			rnd cost[NWAY], y0[NWAY], y1[NWAY];
			rnd sint[NWAY], dy0[NWAY], dy1[NWAY];
			rnd pe[NWAY], po[NWAY];
			for (int j=0; j<NWAY; ++j) {
				cost[j] = vread(ct, j+k);
				sint[j] = -vread(st, j+k);
				y0[j] = vall(al[0]);
				dy0[j] = vall(0.0);
				po[j] = dy0[j];
			}
			if (robert_form) {
				for (int j=0; j<NWAY; ++j) sint[j] *= -sint[j];
			}
			for (int j=0; j<NWAY; ++j) {
				y1[j]  = vall(al[0]*al[1]) * cost[j];
				dy1[j] = vall(al[0]*al[1]) * sint[j];
			}
			for (int j=0; j<NWAY; ++j) {
				pe[j] = -dy1[j] * vall(Tl0[0]);
			}
			al+=2;	l+=2;
			while(l<llim) {
				for (int j=0; j<NWAY; ++j) {
					dy0[j] = vall(al[1])*(cost[j]*dy1[j] + y1[j]*sint[j]) + vall(al[0])*dy0[j];
					y0[j]  = vall(al[1])*(cost[j]*y1[j]) + vall(al[0])*y0[j];
				}
				for (int j=0; j<NWAY; ++j) {
					po[j] -= dy0[j] * vall(Tl0[l-1]);
				}
				for (int j=0; j<NWAY; ++j) {
					dy1[j] = vall(al[3])*(cost[j]*dy0[j] + y0[j]*sint[j]) + vall(al[2])*dy1[j];
					y1[j]  = vall(al[3])*(cost[j]*y0[j]) + vall(al[2])*y1[j];
				}
				for (int j=0; j<NWAY; ++j) {
					pe[j] -= dy1[j] * vall(Tl0[l]);
				}
				al+=4;	l+=2;
			}
			if (l==llim) {
				for (int j=0; j<NWAY; ++j) {
					dy0[j] = vall(al[1])*(cost[j]*dy1[j] + y1[j]*sint[j]) + vall(al[0])*dy0[j];
					y0[j]  = vall(al[1])*cost[j]*y1[j] + vall(al[0])*y0[j];
				}
				for (int j=0; j<NWAY; ++j) {
					po[j] -= dy0[j] * vall(Tl0[l-1]);
				}
			}
		#ifndef SHTNS4MAGIC
			for (int j=0; j<NWAY; ++j) {
				S2D_CSTORE2(BpF, k+j, pe[j], po[j], vall(0), vall(0))
			}
		#else
			for (int j=0; j<NWAY; ++j) {
				if ((k+j)>=nk) break;
				S2D_CSTORE2_4MAGIC((double*)BpF, k+j, pe[j], po[j], vall(0), vall(0));
			}
		#endif
			k+=NWAY;
		} while (k < nk);

	} else {	// im > 0
		const int ms = im*MRES;		// signed m.
		im = abs(im);			// positive im
		v2d VWl[llim*2+4];
		m = im*MRES;
		l = im*(2*(LMAX+1) -m);		// to compute position in NLM array.
		alm = shtns->alm + l+m;
		Tlm -= m;

		{	// convert from vector SH to scalar SH
			double* mx = shtns->mx_stdt + l-m;
			s2d em = vdup(ms);
			v2d tl = ((v2d*)Tlm)[m];
			v2d vs = vdup( 0.0 );
			v2d wt = vdup( 0.0 );
			for (int l=m; l<=llim; l++) {
				s2d mxu = vdup( mx[2*l] );
				s2d mxl = vdup( mx[2*l+1] );		// mxl for next iteration
				vs = addi( vs ,  em*tl );
				v2d wt1 = -mxl*tl;			// wt for next iter
				if (l<llim) {
					tl = ((v2d*)Tlm)[l+1];
					wt -= mxu*tl;
				}
				VWl[2*l]   = vs;
				VWl[2*l+1] = wt;
				vs = vdup( 0.0 );		wt = vdup( 0.0 );
				wt = wt1;
			}
			VWl[2*llim+2] = vs;
			VWl[2*llim+3] = wt;
		}

		k=0;	l=shtns->tm[im];
		while (k<l) {	// polar optimization
		  #ifndef SHTNS4MAGIC
			BtF[k] = vdup(0.0);		BtF[NLAT-l+k] = vdup(0.0);
			BpF[k] = vdup(0.0);		BpF[NLAT-l+k] = vdup(0.0);
		  #else
			BtF[2*k] = vdup(0.0);		BtF[2*k+1] = vdup(0.0);
			BpF[2*k] = vdup(0.0);		BpF[2*k+1] = vdup(0.0);
		  #endif
			++k;
		}

		k = ((unsigned) l) / VSIZE2;
		do {
			al = alm;
			rnd cost[NWAY], y0[NWAY], y1[NWAY];
			rnd ter[NWAY], tei[NWAY], tor[NWAY], toi[NWAY];
			rnd per[NWAY], pei[NWAY], por[NWAY], poi[NWAY];
			for (int j=0; j<NWAY; ++j) {
				cost[j] = vread(st, k+j);
				y0[j] = vall(1.0);
			}
			l=m;
			if (robert_form == 0) l=m-1;
			long int ny = 0;
		  if ((int)llim <= SHT_L_RESCALE_FLY) {
			do {		// sin(theta)^m
				if (l&1) for (int j=0; j<NWAY; ++j) y0[j] *= cost[j];
				for (int j=0; j<NWAY; ++j) cost[j] *= cost[j];
			} while(l >>= 1);
		  } else {
			long int nsint = 0;
			do {		// sin(theta)^m		(use rescaling to avoid underflow)
				if (l&1) {
					for (int j=NWAY-1; j>=0; --j) y0[j] *= cost[j];
					ny += nsint;
					if (vlo(y0[NWAY-1]) < (SHT_ACCURACY+1.0/SHT_SCALE_FACTOR)) {
						ny--;
						for (int j=NWAY-1; j>=0; --j) y0[j] *= vall(SHT_SCALE_FACTOR);
					}
				}
				for (int j=NWAY-1; j>=0; --j) cost[j] *= cost[j];
				nsint += nsint;
				if (vlo(cost[NWAY-1]) < 1.0/SHT_SCALE_FACTOR) {
					nsint--;
					for (int j=NWAY-1; j>=0; --j) cost[j] *= vall(SHT_SCALE_FACTOR);
				}
			} while(l >>= 1);
		  }
			for (int j=0; j<NWAY; ++j) {
				y0[j] *= vall(al[0]);
				cost[j] = vread(ct, j+k);
			}
			for (int j=0; j<NWAY; ++j) {
				y1[j]  = (vall(al[1])*y0[j]) *cost[j];		//	y1[j] = vall(al[1])*cost[j]*y0[j];
				por[j] = vall(0.0);		tei[j] = vall(0.0);
				tor[j] = vall(0.0);		pei[j] = vall(0.0);
				poi[j] = vall(0.0);		ter[j] = vall(0.0);
				toi[j] = vall(0.0);		per[j] = vall(0.0);
			}
			l=m;		al+=2;
			while ((ny<0) && (l<llim)) {		// ylm treated as zero and ignored if ny < 0
				for (int j=0; j<NWAY; ++j) {
					y0[j] = (vall(al[1])*cost[j])*y1[j] + vall(al[0])*y0[j];
				}
				for (int j=0; j<NWAY; ++j) {
					y1[j] = (vall(al[3])*cost[j])*y0[j] + vall(al[2])*y1[j];
				}
				l+=2;	al+=4;
				if (fabs(vlo(y0[NWAY-1])) > SHT_ACCURACY*SHT_SCALE_FACTOR + 1.0) {		// rescale when value is significant
					++ny;
					for (int j=0; j<NWAY; ++j) {
						y0[j] *= vall(1.0/SHT_SCALE_FACTOR);		y1[j] *= vall(1.0/SHT_SCALE_FACTOR);
					}
				}
			}
		  if (ny == 0) {
			while (l<llim) {	// compute even and odd parts
				for (int j=0; j<NWAY; ++j) {	ter[j] += y0[j]  * vr(l);		tei[j] += y0[j] * vi(l);	}
				for (int j=0; j<NWAY; ++j) {	per[j] += y0[j]  * wr(l);		pei[j] += y0[j] * wi(l);	}
				for (int j=0; j<NWAY; ++j) {
					y0[j] = vall(al[1])*(cost[j]*y1[j]) + vall(al[0])*y0[j];
				}
				for (int j=0; j<NWAY; ++j) {	tor[j] += y1[j]  * vr(l+1);		toi[j] += y1[j] * vi(l+1);	}
				for (int j=0; j<NWAY; ++j) {	por[j] += y1[j]  * wr(l+1);		poi[j] += y1[j] * wi(l+1);	}
				for (int j=0; j<NWAY; ++j) {
					y1[j] = vall(al[3])*(cost[j]*y0[j]) + vall(al[2])*y1[j];
				}
				l+=2;	al+=4;
			}
				for (int j=0; j<NWAY; ++j) {	ter[j] += y0[j]  * vr(l);		tei[j] += y0[j] * vi(l);	}
				for (int j=0; j<NWAY; ++j) {	per[j] += y0[j]  * wr(l);		pei[j] += y0[j] * wi(l);	}
			if (l==llim) {
				for (int j=0; j<NWAY; ++j) {	tor[j] += y1[j]  * vr(l+1);		toi[j] += y1[j] * vi(l+1);	}
				for (int j=0; j<NWAY; ++j) {	por[j] += y1[j]  * wr(l+1);		poi[j] += y1[j] * wi(l+1);	}
			}
		  }
		#ifndef SHTNS4MAGIC
			for (int j=0; j<NWAY; ++j) {
				S2D_CSTORE2(BtF, k+j, ter[j], tor[j], tei[j], toi[j])
				S2D_CSTORE2(BpF, k+j, per[j], por[j], pei[j], poi[j])
			}
		#else
			for (int j=0; j<NWAY; ++j) {
				if ((k+j)>=nk) break;
				S2D_CSTORE2_4MAGIC((double*)BtF, k+j, ter[j], tor[j], tei[j], toi[j]);
				S2D_CSTORE2_4MAGIC((double*)BpF, k+j, per[j], por[j], pei[j], poi[j]);
			}
		#endif
			k+=NWAY;
		} while (k < nk);
	}

	#undef tr
	#undef ti
  }

  #endif
