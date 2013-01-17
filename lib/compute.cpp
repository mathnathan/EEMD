
void compute()
{
	int which_imf = 0;

	noiseless_signal = createSignal(200);

	// Loop over noise
	double level = 0.0000;
	int sz;

	// first call
	int nb_calls = 1;
	signals = addNoise(noiseless_signal, level);
	vector<VEC> imfs = compute_imfs(signals);
	printf("size first imfs: %d\n", imfs.size());

	for (int i=0; i < 00; i++) {
		nb_calls++;
		signals = addNoise(noiseless_signal, level);
		vector<VEC> imfs1 = compute_imfs(signals);
		printf("size subsequent imfs: %d\n", imfs1.size());

		for (int i=0; i < imfs.size(); i++) { printf("imf size: %d\n", imfs[i].size());}
		for (int i=0; i < imfs1.size(); i++) { printf("imf1 size: %d\n", imfs1[i].size());}

		#if 1
		// Kludge: calculate in value of imfs.size() and imfs1.size() to avoid memory overload
		sz = imfs.size() < imfs1.size() ? imfs.size() : imfs1.size();

		for (int i=0; i < sz; i++) {
			VEC& imf = imfs[i];
			VEC& imf1 = imfs1[i];
			//printf("sizes: %d, %d\n", imfs1.size(), imfs.size());
			//printf("sizes: %d, %d\n", imf.size(), imf1.size());
			for (int j=0; j < imf.size(); j++) {
				imf[j] += imf1[j];
			}
		}
		#endif
	}

	double dc = (double) nb_calls;
	printf("nb calls: %d\n", nb_calls);

	for (int i=0; i < imfs.size(); i++) {
		VEC& imf = imfs[i];
		for (int j=0; j < imf.size(); j++) {
			imf[j] /= dc;
		}
	}

	// sum all the imfs
	int nb = imfs[0].size();
	vector<double> sum(nb);
	for (int i=0; i < imfs.size(); i++) {
		VEC& imf = imfs[i];
		for (int j=0; j < nb; j++) {
			sum[j] += imf[j];
		}
	}
	for (int j=0; j < nb; j++) {
		sum[j] += residual[j];
	}


	if (which_imf > imfs.size()-1) {
		printf("which_imf reset to imfs.size()-1\n");
		which_imf = imfs.size()-1;
	}

	VEC& imf = imfs[which_imf];
	vector<double> ix(nb);
}

//----------------------------------------------------------------------
