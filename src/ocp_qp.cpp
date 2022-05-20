#include "hpipm-cpp/ocp_qp.hpp"

#include <algorithm>
#include <cstdlib>

namespace hpipm {

ocp_qp::~ocp_qp() {
  if (memory_) {
    free(memory_);
  }
}


std::vector<std::string> ocp_qp::checkSize(const ocp_qp_dim& dim) const {
  std::vector<std::string> err_mgs = dim.checkSize();
  if (!err_mgs.empty()) {
    err_mgs.push_back("Call ocp_qp.checkSize() with correct ocp_qp_dim! Please check the above errors.");
    return err_mgs;
  }
  // dynamics
  if (A.size() != dim.N) err_mgs.push_back("ocp_qp.A.size() must be the same as N!");
  if (B.size() != dim.N) err_mgs.push_back("ocp_qp.B.size() must be the same as N!");
  if (b.size() != dim.N) err_mgs.push_back("ocp_qp.b.size() must be the same as N!");
  for (int i=0; i<dim.N; ++i) {
    if (A[i].rows() != dim.nx[i]) 
      err_mgs.push_back("ocp_qp.A[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
    if (A[i].cols() != dim.nx[i]) 
      err_mgs.push_back("ocp_qp.A[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
    if (B[i].rows() != dim.nx[i]) 
      err_mgs.push_back("ocp_qp.A[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
    if (B[i].cols() != dim.nu[i]) 
      err_mgs.push_back("ocp_qp.B[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nu[" + std::to_string(i) + "]!");
    if (b[i].size() != dim.nx[i]) 
      err_mgs.push_back("ocp_qp.b[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
    if (b[i].cols() != 1) 
      err_mgs.push_back("ocp_qp.b[" + std::to_string(i) + "] must be vector!");
  }
  // costs 
  if (Q.size() != dim.N+1) err_mgs.push_back("ocp_qp.Q.size() must be the same as N+1!");
  if (S.size() != dim.N) err_mgs.push_back("ocp_qp.S.size() must be the same as N!");
  if (R.size() != dim.N) err_mgs.push_back("ocp_qp.R.size() must be the same as N!");
  if (q.size() != dim.N+1) err_mgs.push_back("ocp_qp.q.size() must be the same as N+1!");
  if (r.size() != dim.N) err_mgs.push_back("ocp_qp.r.size() must be the same as N!");
  for (int i=0; i<dim.N; ++i) {
    if (Q[i].rows() != dim.nx[i]) 
      err_mgs.push_back("ocp_qp.Q[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
    if (Q[i].cols() != dim.nx[i]) 
      err_mgs.push_back("ocp_qp.Q[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
    if (S[i].rows() != dim.nu[i]) 
      err_mgs.push_back("ocp_qp.S[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.nu[" + std::to_string(i) + "]!");
    if (S[i].cols() != dim.nx[i]) 
      err_mgs.push_back("ocp_qp.S[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
    if (R[i].rows() != dim.nu[i]) 
      err_mgs.push_back("ocp_qp.R[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.nu[" + std::to_string(i) + "]!");
    if (R[i].cols() != dim.nu[i]) 
      err_mgs.push_back("ocp_qp.R[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nu[" + std::to_string(i) + "]!");
    if (q[i].size() != dim.nx[i]) 
      err_mgs.push_back("ocp_qp.q[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
    if (q[i].cols() != 1) 
      err_mgs.push_back("ocp_qp.q[" + std::to_string(i) + "] must be vector!");
    if (r[i].size() != dim.nu[i]) 
      err_mgs.push_back("ocp_qp.r[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nu[" + std::to_string(i) + "]!");
    if (r[i].cols() != 1) 
      err_mgs.push_back("ocp_qp.r[" + std::to_string(i) + "] must be vector!");
  }
  if (Q[dim.N].rows() != dim.nx[dim.N]) 
    err_mgs.push_back("ocp_qp.Q[" + std::to_string(dim.N) + "].rows() must be the same as ocp_qp_dim.nx[" + std::to_string(dim.N) + "]!");
  if (Q[dim.N].cols() != dim.nx[dim.N]) 
    err_mgs.push_back("ocp_qp.Q[" + std::to_string(dim.N) + "].cols() must be the same as ocp_qp_dim.nx[" + std::to_string(dim.N) + "]!");
  if (q[dim.N].size() != dim.nx[dim.N]) 
    err_mgs.push_back("ocp_qp.q[" + std::to_string(dim.N) + "].size() must be the same as ocp_qp_dim.nx[" + std::to_string(dim.N) + "]!");
  if (q[dim.N].cols() != 1) 
    err_mgs.push_back("ocp_qp.q[" + std::to_string(dim.N) + "] must be vector!");
  // box constraints
  if (idxbx.empty()) {
    if (!lbx.empty()) err_mgs.push_back("lbx must be empty because idxbx is empty!");
    if (!ubx.empty()) err_mgs.push_back("ubx must be empty because idxbx is empty!");
  }
  else {
    if (idxbx.size() != dim.N+1) err_mgs.push_back("ocp_qp.idxbx.size() must be N+1 or 0!");
    if (lbx.size() != dim.N+1) err_mgs.push_back("ocp_qp.lbx.size() must be N+1 or 0!");
    if (ubx.size() != dim.N+1) err_mgs.push_back("ocp_qp.ubx.size() must be N+1 or 0!");
    for (int i=0; i<=dim.N; ++i) {
      if (idxbx[i].size() != dim.nbx[i]) 
        err_mgs.push_back("ocp_qp.idxbx[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nbx[" + std::to_string(i) + "]!");
      if (lbx[i].size() != dim.nbx[i]) 
        err_mgs.push_back("ocp_qp.lbx[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nbx[" + std::to_string(i) + "]!");
      if (ubx[i].size() != dim.nbx[i]) 
        err_mgs.push_back("ocp_qp.ubx[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nbx[" + std::to_string(i) + "]!");
    }
  }
  if (idxbu.empty()) {
    if (!lbu.empty()) err_mgs.push_back("lbu must be empty because idxbu is empty!");
    if (!ubu.empty()) err_mgs.push_back("ubu must be empty because idxbu is empty!");
  }
  else {
    if (idxbu.size() != dim.N)  err_mgs.push_back("ocp_qp.idxbu.size() must be N or 0!");
    if (lbu.size() != dim.N) err_mgs.push_back("ocp_qp.lbu.size() must be N or 0!");
    if (ubu.size() != dim.N) err_mgs.push_back("ocp_qp.ubu.size() must be N or 0!");
    for (int i=0; i<dim.N; ++i) {
      if (idxbu[i].size() != dim.nbu[i]) 
        err_mgs.push_back("ocp_qp.idxbu[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nbu[" + std::to_string(i) + "]!");
      if (lbu[i].size() != dim.nbu[i]) 
        err_mgs.push_back("ocp_qp.lbx[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nbu[" + std::to_string(i) + "]!");
      if (ubu[i].size() != dim.nbu[i]) 
        err_mgs.push_back("ocp_qp.ubx[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nbu[" + std::to_string(i) + "]!");
    }
  }
  // constraints
  if (C.empty()) {
    if (!D.empty()) err_mgs.push_back("D must be empty because C is empty!");
    if (!lg.empty()) err_mgs.push_back("lg must be empty because C is empty!");
    if (!ug.empty()) err_mgs.push_back("ug must be empty because C is empty!");
  }
  else {
    if (C.size() != dim.N+1) err_mgs.push_back("ocp_qp.C.size() must be N+1 or 0!");
    if (D.size() != dim.N) err_mgs.push_back("ocp_qp.D.size() must be N or 0!");
    if (lg.size() != dim.N) err_mgs.push_back("ocp_qp.lg.size() must be N+1 or 0!");
    if (ug.size() != dim.N) err_mgs.push_back("ocp_qp.ug.size() must be N+1 or 0!");
    for (int i=0; i<dim.N; ++i) {
      if (C[i].rows() != dim.ng[i]) 
        err_mgs.push_back("ocp_qp.C[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.ng[" + std::to_string(i) + "]!");
      if (C[i].cols() != dim.nx[i]) 
        err_mgs.push_back("ocp_qp.C[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nx[" + std::to_string(i) + "]!");
      if (D[i].rows() != dim.ng[i]) 
        err_mgs.push_back("ocp_qp.D[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.ng[" + std::to_string(i) + "]!");
      if (D[i].cols() != dim.nu[i]) 
        err_mgs.push_back("ocp_qp.D[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nu[" + std::to_string(i) + "]!");
      if (lg[i].size() != dim.ng[i]) 
        err_mgs.push_back("ocp_qp.lg[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.ng[" + std::to_string(i) + "]!");
      if (lg[i].cols() != 1) 
        err_mgs.push_back("ocp_qp.lg[" + std::to_string(i) + "] must be a vector!");
      if (ug[i].size() != dim.ng[i]) 
        err_mgs.push_back("ocp_qp.ug[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.ng[" + std::to_string(i) + "]!");
      if (ug[i].cols() != 1) 
        err_mgs.push_back("ocp_qp.ug[" + std::to_string(i) + "] must be a vector!");
    }
    if (C[dim.N].rows() != dim.ng[dim.N]) 
      err_mgs.push_back("ocp_qp.C[" + std::to_string(dim.N) + "].rows() must be the same as ocp_qp_dim.ng[" + std::to_string(dim.N) + "]!");
    if (C[dim.N].cols() != dim.nx[dim.N]) 
      err_mgs.push_back("ocp_qp.C[" + std::to_string(dim.N) + "].cols() must be the same as ocp_qp_dim.nx[" + std::to_string(dim.N) + "]!");
    if (lg[dim.N].size() != dim.ng[dim.N]) 
      err_mgs.push_back("ocp_qp.lg[" + std::to_string(dim.N) + "].size() must be the same as ocp_qp_dim.ng[" + std::to_string(dim.N) + "]!");
    if (lg[dim.N].cols() != 1) 
      err_mgs.push_back("ocp_qp.lg[" + std::to_string(dim.N) + "] must be a vector!");
    if (ug[dim.N].size() != dim.ng[dim.N]) 
      err_mgs.push_back("ocp_qp.ug[" + std::to_string(dim.N) + "].size() must be the same as ocp_qp_dim.ng[" + std::to_string(dim.N) + "]!");
    if (ug[dim.N].cols() != 1) 
      err_mgs.push_back("ocp_qp.ug[" + std::to_string(dim.N) + "] must be a vector!");
  }
  if (Zl.empty()) {
    if (!Zu.empty()) err_mgs.push_back("Zu must be empty because Zu is empty!");
    if (!zl.empty()) err_mgs.push_back("lg must be empty because zl is empty!");
    if (!zu.empty()) err_mgs.push_back("lg must be empty because zu is empty!");
  }
  else {
    for (int i=0; i<=dim.N; ++i) {
      if (Zu[i].rows() != dim.nsg[i]) 
        err_mgs.push_back("ocp_qp.Zu[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.nsg[" + std::to_string(i) + "]!");
      if (Zu[i].cols() != dim.nsg[i]) 
        err_mgs.push_back("ocp_qp.Zu[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nsg[" + std::to_string(i) + "]!");
      if (Zl[i].rows() != dim.nsg[i]) 
        err_mgs.push_back("ocp_qp.Zl[" + std::to_string(i) + "].rows() must be the same as ocp_qp_dim.nsg[" + std::to_string(i) + "]!");
      if (Zl[i].cols() != dim.nsg[i]) 
        err_mgs.push_back("ocp_qp.Zl[" + std::to_string(i) + "].cols() must be the same as ocp_qp_dim.nsg[" + std::to_string(i) + "]!");
      if (zu[i].size() != dim.nsg[i]) 
        err_mgs.push_back("ocp_qp.zu[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nsg[" + std::to_string(i) + "]!");
      if (zu[i].cols() != 1) 
        err_mgs.push_back("ocp_qp.zu[" + std::to_string(i) + "] must be a vector!");
      if (zl[i].size() != dim.nsg[i]) 
        err_mgs.push_back("ocp_qp.zl[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nsg[" + std::to_string(i) + "]!");
      if (zl[i].cols() != 1) 
        err_mgs.push_back("ocp_qp.zl[" + std::to_string(i) + "] must be a vector!");
    }
  }
  if (idxs.empty()) {
    if (!lls.empty()) err_mgs.push_back("lls must be empty because idxs is empty!");
    if (!lus.empty()) err_mgs.push_back("lus must be empty because idxs is empty!");
  }
  else {
    for (int i=0; i<=dim.N; ++i) {
      if (idxs.size() != dim.N+1) err_mgs.push_back("ocp_qp.idxs.size() must be N+1 or 0!");
      if (lls.size() != dim.N+1) err_mgs.push_back("ocp_qp.lls.size() must be N+1 or 0!");
      if (lus.size() != dim.N+1) err_mgs.push_back("ocp_qp.lus.size() must be N+1 or 0!");
      for (int i=0; i<=dim.N; ++i) {
        if (idxs[i].size() != dim.nsbx[i]) 
          err_mgs.push_back("ocp_qp.idxs[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nsbx[" + std::to_string(i) + "]!");
        if (lls[i].size() != dim.nsbx[i]) 
          err_mgs.push_back("ocp_qp.lls[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nsbx[" + std::to_string(i) + "]!");
        if (lus[i].size() != dim.nsbx[i]) 
          err_mgs.push_back("ocp_qp.lus[" + std::to_string(i) + "].size() must be the same as ocp_qp_dim.nsbx[" + std::to_string(i) + "]!");
      }
    }
  }
  return err_mgs;
}


void ocp_qp::create_hpipm_data(ocp_qp_dim& dim) {
  const hpipm_size_t new_memsize = d_ocp_qp_memsize(dim.to_hpipm());
  if (memory_ && new_memsize >= memsize_) {
    free(memory_);
    memory_ = nullptr;
  }
  memsize_ = std::max(memsize_, new_memsize);
  if (!memory_) {
    memory_ = malloc(memsize_);
    d_ocp_qp_create(dim.to_hpipm(), &ocp_qp_hpipm_, memory_);
  }
  A_ptr_.clear();
  B_ptr_.clear();
  b_ptr_.clear();
  Q_ptr_.clear();
  S_ptr_.clear();
  R_ptr_.clear();
  q_ptr_.clear();
  r_ptr_.clear();
  idxbx_ptr_.clear();
  lbx_ptr_.clear();
  ubx_ptr_.clear();
  idxbu_ptr_.clear();
  lbu_ptr_.clear();
  ubu_ptr_.clear();
  C_ptr_.clear();
  D_ptr_.clear();
  lg_ptr_.clear();
  ug_ptr_.clear();
  Zl_ptr_.clear();
  Zu_ptr_.clear();
  zl_ptr_.clear();
  zu_ptr_.clear();
  idxs_ptr_.clear();
  lls_ptr_.clear();
  lus_ptr_.clear();

  A_ptr_.resize(dim.N);
  B_ptr_.resize(dim.N);
  b_ptr_.resize(dim.N);
  Q_ptr_.resize(dim.N+1);
  S_ptr_.resize(dim.N+1);
  R_ptr_.resize(dim.N+1);
  q_ptr_.resize(dim.N+1);
  r_ptr_.resize(dim.N+1);
  idxbx_ptr_.resize(dim.N+1);
  lbx_ptr_.resize(dim.N+1);
  ubx_ptr_.resize(dim.N+1);
  idxbu_ptr_.resize(dim.N+1);
  lbu_ptr_.resize(dim.N+1);
  ubu_ptr_.resize(dim.N+1);
  C_ptr_.resize(dim.N+1);
  D_ptr_.resize(dim.N+1);
  lg_ptr_.resize(dim.N+1);
  ug_ptr_.resize(dim.N+1);
  Zl_ptr_.resize(dim.N+1);
  Zu_ptr_.resize(dim.N+1);
  zl_ptr_.resize(dim.N+1);
  zu_ptr_.resize(dim.N+1);
  idxs_ptr_.resize(dim.N+1);
  lls_ptr_.resize(dim.N+1);
  lus_ptr_.resize(dim.N+1);

  // dynamics
  for (int i=0; i<dim.N; ++i) {
    A_ptr_[i] = A[i].data();
    B_ptr_[i] = B[i].data();
    b_ptr_[i] = b[i].data();
  }
  // costs
  for (int i=0; i<dim.N; ++i) {
    Q_ptr_[i] = Q[i].data();
    S_ptr_[i] = S[i].data();
    R_ptr_[i] = R[i].data();
    q_ptr_[i] = q[i].data();
    r_ptr_[i] = r[i].data();
  }
  Q_ptr_[dim.N] = Q[dim.N].data();
  q_ptr_[dim.N] = q[dim.N].data();
  // box constraints on state
  if (idxbx.empty() || lbx.empty() || ubx.empty()) {
    idxbx.clear();
    lbx.clear();
    ubx.clear();
    idxbx.resize(dim.N+1);
    lbx.resize(dim.N+1);
    ubx.resize(dim.N+1);
  }
  for (int i=0; i<=dim.N; ++i) {
    idxbx_ptr_[i] = idxbx[i].data();
    lbx_ptr_[i] = lbx[i].data();
    ubx_ptr_[i] = ubx[i].data();
  }
  // box constraints on control input
  if (idxbu.empty() || lbu.empty() || ubu.empty()) {
    idxbu.clear();
    lbu.clear();
    ubu.clear();
    idxbu.resize(dim.N+1);
    lbu.resize(dim.N+1);
    ubu.resize(dim.N+1);
  }
  for (int i=0; i<dim.N; ++i) {
    idxbu_ptr_[i] = idxbu[i].data();
    lbu_ptr_[i] = lbu[i].data();
    ubu_ptr_[i] = ubu[i].data();
  }
  // constraints
  if (C.empty() || D.empty() || lg.empty() || ug.empty()) {
    C.clear();
    D.clear();
    lg.clear();
    ug.clear();
    C.resize(dim.N+1);
    D.resize(dim.N+1);
    lg.resize(dim.N+1);
    ug.resize(dim.N+1);
  }
  for (int i=0; i<dim.N; ++i) {
    C_ptr_[i] = C[i].data();
    D_ptr_[i] = D[i].data();
    lg_ptr_[i] = lg[i].data();
    ug_ptr_[i] = ug[i].data();
  }
  C_ptr_[dim.N] = C[dim.N].data();
  lg_ptr_[dim.N] = lg[dim.N].data();
  ug_ptr_[dim.N] = ug[dim.N].data();
  // soft constraints
  if (Zl.empty() || Zu.empty() || zl.empty() || zu.empty()) {
    Zl.clear();
    Zu.clear();
    zl.clear();
    zu.clear();
    Zl.resize(dim.N+1);
    Zu.resize(dim.N+1);
    zl.resize(dim.N+1);
    zu.resize(dim.N+1);
  }
  for (int i=0; i<=dim.N; ++i) {
    Zl_ptr_[i] = Zu[i].data();
    Zu_ptr_[i] = Zu[i].data();
    zl_ptr_[i] = zl[i].data();
    zu_ptr_[i] = zu[i].data();
  }
  // TODO: what is this??
  if (idxs.empty() || lls.empty() || lus.empty()) {
    idxs.clear();
    lls.clear();
    lus.clear();
    idxs.resize(dim.N+1);
    lls.resize(dim.N+1);
    lus.resize(dim.N+1);
  }
  for (int i=0; i<=dim.N; ++i) {
    idxs_ptr_[i] = idxs[i].data();
    lls_ptr_[i] = lls[i].data();
    lus_ptr_[i] = lus[i].data();
  }
  d_ocp_qp_set_all(A_ptr_.data(), B_ptr_.data(), b_ptr_.data(), 
                   Q_ptr_.data(), S_ptr_.data(), R_ptr_.data(), q_ptr_.data(), r_ptr_.data(), 
                   idxbx_ptr_.data(), lbx_ptr_.data(), ubx_ptr_.data(),
                   idxbu_ptr_.data(), lbu_ptr_.data(), ubu_ptr_.data(),
                   C_ptr_.data(), D_ptr_.data(), lg_ptr_.data(), ug_ptr_.data(), 
                   Zl_ptr_.data(), Zu_ptr_.data(), zl_ptr_.data(), zu_ptr_.data(), 
                   idxs_ptr_.data(), lls_ptr_.data(), lus_ptr_.data(), &ocp_qp_hpipm_);
}

} // namespace hpipm
