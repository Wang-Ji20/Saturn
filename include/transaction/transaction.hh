//===------------------------------------------===
// Saturn 2023
//
// Identification: include/transaction/transaction.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

namespace saturn {

class TransactionManager;

class Transaction {
  friend class TransactionManager;
public:
  Transaction(TransactionManager &manager) : manager_(manager) {}
  ~Transaction() = default;

  void Begin() {}
  void Commit() {}
  void Abort() {}
private:
  TransactionManager &manager_;
};

} // namespace saturn
